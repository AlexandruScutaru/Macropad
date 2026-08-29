#include "KeypadService.h"
#include "app/AppSettings.h"
#include "action-handlers/IActionHandler.h"

#include <nlohmann/json.hpp>

#include <QDebug>

#include <algorithm>


using json = nlohmann::json;
using namespace Keypad;

static OptionType GetInternalOptionType(action_handlers::OptionType type) {
    switch (type) {
        case action_handlers::OptionType::String: return OptionType::String;
        case action_handlers::OptionType::FilePath:
        case action_handlers::OptionType::FolderPath: return OptionType::Path;
        default: return OptionType::Unknown;
    }
}


KeypadService::KeypadService(AppSettings* appSettings, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
    , mTaskRunner(3)
{
    qDebug() << "KeypadService::KeypadService";

    // will either get it from local cache or make an async request to get it from cloud if needed
    loadSavedProfile();
}

KeypadService::~KeypadService() {
    qDebug() << "KeypadService::~KeypadService";
}


void KeypadService::registerHandler(const action_handlers::IActionHandlerPtr& handler) {
    if (!handler) {
        return;
    }

    const auto& handlerId = handler->id();
    if (const auto& it = mActionHandlers.find(handlerId); it != mActionHandlers.end()) {
        return;
    }

    mActionHandlers[handlerId] = handler;

    auto section = handler->getActions();

    Section sectionEntry;
    sectionEntry.id = QString::fromStdString(handlerId);
    sectionEntry.displayName = QString::fromStdString(section.displayName);

    sectionEntry.iconName = QString::fromStdString(section.iconName);
    if (sectionEntry.iconName.isEmpty()) {
        sectionEntry.iconName = "sliders.svg";
    }

    sectionEntry.actions.reserve(section.actions.size());

    for (const auto& action: section.actions) {
        Action actionEntry;
        actionEntry.id = QString::fromStdString(action.id);
        actionEntry.sectionId = QString::fromStdString(handlerId);
        actionEntry.displayName = QString::fromStdString(action.displayName);
        actionEntry.tooltip = QString::fromStdString(action.tooltip);

        actionEntry.iconName = QString::fromStdString(action.iconName);
        if (actionEntry.iconName.isEmpty()) {
            actionEntry.iconName = "sliders.svg";
        }

        actionEntry.configs.reserve(action.configs.size());

        for (const auto& config: action.configs) {
            Config configEntry;
            configEntry.name = QString::fromStdString(config.name);
            configEntry.displayName = QString::fromStdString(config.displayName);
            configEntry.tooltip = QString::fromStdString(config.tooltip);
            configEntry.type = GetInternalOptionType(config.type);
            configEntry.wantFolder = config.type == action_handlers::OptionType::FolderPath;

            actionEntry.configs.push_back(configEntry);
        }

        sectionEntry.actions.push_back(actionEntry.id);
        mAvailableActions.actionsMap[actionEntry.id] = actionEntry;
    }

    mAvailableActions.sections.push_back(sectionEntry);

    emit availableActionsChanged(mAvailableActions);
    loadSavedProfile();
}

void KeypadService::onProfileLoaded(const Keypad::Profile& profile) {
    mCurrentProfile = profile;
    emit profileChanged(mCurrentProfile);
}

void KeypadService::onActionAssignRequested(int layer, int key, const QString& actionId) {
    try {
        if (const auto actionInfo = mAvailableActions.getAction(actionId); actionInfo != std::nullopt) {
            //TODO: update this so that profile data is saved more granularly
            // to not dump the entire thing just for some fields of it
            mCurrentProfile.layers[layer].actions[key] = *actionInfo;
            saveProfile();

            emit actionAssigned(layer, key, *actionInfo);
        }
    } catch(...) {
        qWarning() << "An error ocurred accessing the action to be assigned:" << layer << key << actionId;
    }
}

void KeypadService::onKeySelected(int layer, int key) {
    try {
        const auto& action = mCurrentProfile.layers[layer].actions[key];
        emit actionConfigChanged(layer, key, action);
    } catch(...) {
        qWarning() << "An error ocurred accessing the configuration of the selected key: " << layer << key;
    }
}

void KeypadService::onKeyTriggered(int layer, int key) {
    try {
        const auto& action = mCurrentProfile.layers[layer].actions[key];
        if (action.id.isEmpty()) {
            return;
        }

        if (const auto& it = mActionHandlers.find(action.sectionId.toStdString()); it != mActionHandlers.end()) {
            if (!it->second) {
                return;
            }

            nlohmann::json actionPayload;
            actionPayload["id"] = action.id.toStdString();
            for (const auto& configEntry: action.configs) {
                WriteVariant(actionPayload, configEntry.name, configEntry.type, configEntry.value);
            }

            auto callback = []() {
                qDebug() << "finished action handling";
            };

            mTaskRunner.run([handler = it->second, actionPayload, cb = mTaskRunner.mainThreadProxy(callback)]() {
                handler->handleAction(actionPayload.dump());
                cb();
            });
        }
    } catch(...) {
        qWarning() << "An error ocurred accessing the configuration of the triggered key: " << layer << key;
    }
}

void KeypadService::onConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value) {
    try {
        auto& action = mCurrentProfile.layers[layer].actions[key];
        if (auto config = std::find_if(action.configs.begin(), action.configs.end(), [name](const auto& config) { return config.name == name; });
            config != action.configs.end())
        {
            config->value = value;
            saveProfile();

            emit actionConfigOptionChanged(layer, key, name, value);
        }
    } catch(...) {
        qWarning() << "An error ocurred updating the key config option:" << layer << key << name;
    }
}


void KeypadService::loadSavedProfile() {
    Keypad::Profile profile = {};

    try {
        const auto profileStr = mAppSettings->profileData();
        json profileJson = json::parse(profileStr.toStdString());
        profile.name = QString::fromStdString(profileJson.at("name").get<std::string>());

        const auto& layersJson = profileJson.at("layers");
        profile.layers.resize(layersJson.size());
        for (const auto& layerJson: layersJson) {
            auto& layer = profile.layers[layerJson.at("index").get<int>()];
            layer.color = QString::fromStdString(layerJson.value<std::string>("color", "transparent"));

            const auto& actionsJson = layerJson.at("actions");
            layer.actions.resize(actionsJson.size());
            for (const auto& actionJson: actionsJson) {
                auto& action = layer.actions[actionJson.at("index").get<int>()];

                const auto actionId = QString::fromStdString(actionJson.at("id").get<std::string>());
                if (actionId.isEmpty()) {
                    continue;
                }

                // not all details are saved in the profile, only the id
                // for that the current available actions are queried for the rest of the data
                if (const auto& actionInfo = mAvailableActions.getAction(actionId); actionInfo != std::nullopt) {
                    action.id = actionId;
                    action.sectionId = actionInfo->sectionId;
                    action.displayName = actionInfo->displayName;
                    action.tooltip = actionInfo->tooltip;
                    action.iconName = actionInfo->iconName;

                    const auto& configsJson = actionJson.contains("configs") ? actionJson.at("configs") : json::array();
                    action.configs.resize(configsJson.size());
                    if (action.configs.size() == actionInfo->configs.size()) {
                        for (const auto& configJson: configsJson) {
                            int configIndex = configJson.at("index").get<int>();
                            auto& config = action.configs[configIndex];
                            config.name = QString::fromStdString(configJson.at("name").get<std::string>());
                            config.displayName = actionInfo->configs[configIndex].displayName;
                            config.tooltip = actionInfo->configs[configIndex].tooltip;
                            config.type = actionInfo->configs[configIndex].type;
                            config.value = ReadVariant(configJson, "value", config.type);
                            config.wantFolder = actionInfo->configs[configIndex].wantFolder;
                        }
                    }
                }
            }
        }
    } catch (const json::exception& e) {
        qWarning() << "Failed to parse profile JSON:" << e.what();
        profile = {};
    }

    if (profile.layers.empty()) {
        // TODO: set this as a configuration from upper levels
        const size_t ACTIONS_PER_LAYER = 9;
        const auto actions = std::vector<Keypad::Action>(ACTIONS_PER_LAYER);
        profile.layers.emplace_back("#00ff00", actions);
        profile.name = "Default profile";
    }

    onProfileLoaded(profile);
}

void KeypadService::saveProfile() {
    try {
        json profileJson;
        profileJson["name"] = mCurrentProfile.name.toStdString();

        for (auto layerIndex = 0; layerIndex < mCurrentProfile.layers.size(); layerIndex++) {
            const auto& layer = mCurrentProfile.layers[layerIndex];
            json layerJson;
            layerJson["index"] = layerIndex;
            layerJson["color"] = layer.color.toStdString();

            for (auto actionIndex = 0; actionIndex < layer.actions.size(); actionIndex++) {
                const auto& action = layer.actions[actionIndex];
                json actionJson;
                actionJson["index"] = actionIndex;
                actionJson["id"] = action.id.toStdString();

                for (auto configIndex = 0; configIndex < action.configs.size(); configIndex++) {
                    const auto& config = action.configs[configIndex];
                    json configJson;
                    configJson["index"] = configIndex;
                    configJson["name"] = config.name.toStdString();
                    WriteVariant(configJson, "value", config.type, config.value);
                    actionJson["configs"].push_back(configJson);
                }

                layerJson["actions"].push_back(actionJson);
            }

            profileJson["layers"].push_back(layerJson);
        }

        const std::string profileStr = profileJson.dump();
        mAppSettings->saveProfileData(QString::fromStdString(profileStr));
    } catch (const json::exception& e) {
        qWarning() << "Failed to save profile JSON:" << e.what();
    }
}

Keypad::AvailableActions KeypadService::getAvailableActions() const {
    return mAvailableActions;
}

Keypad::Profile KeypadService::getCurrentProfile() const {
    return mCurrentProfile;
}

QVariant KeypadService::ReadVariant(const nlohmann::json& json, const QString& fieldName, Keypad::OptionType type) {
    switch (type) {
        case Keypad::OptionType::String:
        case Keypad::OptionType::Path:
            return QString::fromStdString(json.value<std::string>(fieldName.toStdString(), ""));
        default:
            break;
    }

    return {};
}

void KeypadService::WriteVariant(nlohmann::json& json, const QString& fieldName, Keypad::OptionType type, const QVariant& variant) {
    switch (type) {
        case Keypad::OptionType::String:
        case Keypad::OptionType::Path:
            json[fieldName.toStdString()] = variant.toString().toStdString();
            break;
        default:
            break;
    }
}
