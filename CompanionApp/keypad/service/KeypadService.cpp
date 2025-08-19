#include "KeypadService.h"
#include "AppSettings.h"
#include "../actions/SystemActions.h"
#include "../model/ActionConfigListModel.h"

#include <nlohmann/json.hpp>
#include <QDebug>

#include <algorithm>

using json = nlohmann::json;


KeypadService::KeypadService(QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
{
    qDebug() << "KeypadService::KeypadService";
    loadAvailableActions();
}

KeypadService::~KeypadService() {
    qDebug() << "KeypadService::~KeypadService";
}


const Keypad::AvailableActions& KeypadService::getAvailableActions() const {
    return mAvailableActions;
}

const Keypad::Profile& KeypadService::getCurrentProfile() const {
    return mCurrentProfile;
}

void KeypadService::onActionAssignRequested(int layer, int key, const QString& actionName) {
    try {
        if (const auto actionInfo = mAvailableActions.getAction(actionName); actionInfo != std::nullopt) {
            mCurrentProfile.layers[layer].actions[key] = *actionInfo;
            //TODO: update code so that profile data is saved more granurarly so as to not dump the entire thing for just some parts of it
            saveProfile(mCurrentProfile);
            emit actionAssigned(layer, key, *actionInfo);
        }
    } catch(...) {
        qWarning() << "An error ocurred accessing the action to be assigned:" << layer << key << actionName;
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
        if (action.name.isEmpty()) {
            return;
        }
        emit actionTriggered(action);
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
            saveProfile(mCurrentProfile);
            emit actionConfigChanged(layer, key, action);
        }
    } catch(...) {
        qWarning() << "An error ocurred updating the key config option:" << layer << key << name;
    }
}

void KeypadService::loadSavedProfile() {
    mCurrentProfile = {};

    try {
        const auto profileStr = mAppSettings->profileData();
        json profileJson = json::parse(profileStr.toStdString());
        mCurrentProfile.name = QString::fromStdString(profileJson.at("name").get<std::string>());

        const auto& layersJson = profileJson.at("layers");
        mCurrentProfile.layers.resize(layersJson.size());
        for (const auto& layerJson: layersJson) {
            auto& layer = mCurrentProfile.layers[layerJson.at("index").get<int>()];
            layer.color = QString::fromStdString(layerJson.value<std::string>("color", "transparent"));

            const auto& actionsJson = layerJson.at("actions");
            layer.actions.resize(actionsJson.size());
            for (const auto& actionJson: actionsJson) {
                auto& action = layer.actions[actionJson.at("index").get<int>()];

                const auto actionName = QString::fromStdString(actionJson.at("name").get<std::string>());
                if (const auto& actionInfo = mAvailableActions.getAction(actionName); actionInfo != std::nullopt) {
                    action.name = actionName;
                    action.displayName = actionInfo->displayName;
                    action.tooltip = actionInfo->tooltip;
                    action.icon = actionInfo->icon;

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
                            config.value = JsonReadVariant(configJson, "value", config.type);
                        }
                    }
                }
            }
        }
    } catch (const json::exception& e) {
        qWarning() << "Failed to parse profile JSON:" << e.what();
        mCurrentProfile = {};
    }

    if (mCurrentProfile.layers.empty()) {
        // TODO: set this as a configuration from upper levels
        const size_t ACTIONS_PER_LAYER = 9;
        const auto actions = std::vector<Keypad::Action>(ACTIONS_PER_LAYER);
        mCurrentProfile.layers.emplace_back("#00ff00", actions );
        mCurrentProfile.name = "Default profile";
    }

    emit profileLoaded(mCurrentProfile);
}

void KeypadService::saveProfile(const Keypad::Profile& profile) {
    try {
        json profileJson;
        profileJson["name"] = profile.name.toStdString();

        for (auto layerIndex = 0; layerIndex < profile.layers.size(); layerIndex++) {
            const auto& layer = profile.layers[layerIndex];
            json layerJson;
            layerJson["index"] = layerIndex;
            layerJson["color"] = layer.color.toStdString();

            for (auto actionIndex = 0; actionIndex < layer.actions.size(); actionIndex++) {
                const auto& action = layer.actions[actionIndex];
                json actionJson;
                actionJson["index"] = actionIndex;
                actionJson["name"] = action.name.toStdString();

                for (auto configIndex = 0; configIndex < action.configs.size(); configIndex++) {
                    const auto& config = action.configs[configIndex];
                    json configJson;
                    configJson["index"] = configIndex;
                    configJson["name"] = config.name.toStdString();
                    JsonWriteVariant(configJson, "value", config.type, config.value);
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


void KeypadService::loadAvailableActions() {
    Keypad::Actions::InsertSystemActions(mAvailableActions);
}


QVariant KeypadService::JsonReadVariant(const nlohmann::json& json, const std::string& fieldName, Keypad::OptionType type) {
    switch (type) {
        case Keypad::OptionType::String:
            return QString::fromStdString(json.value<std::string>(fieldName, ""));
        default:
            // no-op
            break;
    }

    return {};
}

void KeypadService::JsonWriteVariant(nlohmann::json& json, const std::string& fieldName, Keypad::OptionType type, const QVariant& variant) {
    switch (type) {
        case Keypad::OptionType::String:
            json[fieldName] = variant.toString().toStdString();
        default:
            // no-op
            break;
    }
}
