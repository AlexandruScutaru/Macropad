#include "KeypadService.h"
#include "AppSettings.h"
#include "../model/ActionConfigListModel.h"
#include "../Utils.h"

#include <nlohmann/json.hpp>
#include <QDebug>

#include <algorithm>

using json = nlohmann::json;


KeypadService::KeypadService(QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
{
    qDebug() << "KeypadService::KeypadService";
}

KeypadService::~KeypadService() {
    qDebug() << "KeypadService::~KeypadService";
}


void KeypadService::loadSavedProfile(const Keypad::AvailableActions& availableActions) {
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
                // not all details are saved in the profile, only the id
                // for that the current available actions are queried for the rest of the data
                if (const auto& actionInfo = availableActions.getAction(actionId); actionInfo != std::nullopt) {
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
                            config.value = utils::json::ReadVariant(configJson, "value", config.type);
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

    emit profileLoaded(profile);
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
                actionJson["id"] = action.id.toStdString();

                for (auto configIndex = 0; configIndex < action.configs.size(); configIndex++) {
                    const auto& config = action.configs[configIndex];
                    json configJson;
                    configJson["index"] = configIndex;
                    configJson["name"] = config.name.toStdString();
                    utils::json::WriteVariant(configJson, "value", config.type, config.value);
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
