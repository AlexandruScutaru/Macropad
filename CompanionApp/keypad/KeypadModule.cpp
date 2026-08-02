#include "KeypadModule.h"
#include "KeypadTypes.h"
#include "controller/KeypadController.h"
#include "service/KeypadService.h"
#include "AppSettings.h"
#include "action-handlers/IActionHandler.h"
#include "Utils.h"

#include <nlohmann/json.hpp>

#include <QDebug>

#include <algorithm>

using namespace Keypad;


static OptionType GetInternalOptionType(action_handlers::OptionType type) {
    switch (type) {
        case action_handlers::OptionType::String: return OptionType::String;
        case action_handlers::OptionType::FilePath:
        case action_handlers::OptionType::FolderPath: return OptionType::Path;
        default: return OptionType::Unknown;
    }
}


KeypadModule::KeypadModule(QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
    , mTaskRunner(3)
{
    qDebug() << "KeypadModule::KeypadModule";

    // to be abstracted and injected from outside
    mService = new KeypadService(appSettings, this);

    mActionConfigController = new ActionConfigController(this);
    mAvailableActionsController = new AvailableActionsController(this);
    mKeypadController = new KeypadController(this);

    QObject::connect(mService, &KeypadService::profileLoaded, this, &KeypadModule::onProfileLoaded);
    QObject::connect(mKeypadController, &KeypadController::actionAssignRequested, this, &KeypadModule::onActionAssignRequested);
    QObject::connect(mKeypadController, &KeypadController::keySelected, this, &KeypadModule::onKeySelected);
    QObject::connect(mKeypadController, &KeypadController::keyTriggered, this, &KeypadModule::onKeyTriggered);
    QObject::connect(mActionConfigController, &ActionConfigController::configOptionChanged,  this, &KeypadModule::onConfigOptionChanged);

    // will either get it from local cache or make an async request to get it from cloud if needed
    mService->loadSavedProfile(mAvailableActions);
}

KeypadModule::~KeypadModule() {
    qDebug() << "KeypadModule::~KeypadModule";
}


ActionConfigController* KeypadModule::getActionConfigController() {
    return mActionConfigController;
}

AvailableActionsController* KeypadModule::getAvailableActionsController() {
    return mAvailableActionsController;
}

KeypadController* KeypadModule::getKeypadController() {
    return mKeypadController;
}

void KeypadModule::registerHandler(const IActionHandlerPtr& handler) {
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

    mAvailableActionsController->onAvailableActionsChanged(mAvailableActions);
    mService->loadSavedProfile(mAvailableActions);
}


void KeypadModule::onProfileLoaded(const Keypad::Profile& profile) {
    mCurrentProfile = profile;
    mKeypadController->onProfileChanged(mCurrentProfile);
}

void KeypadModule::onActionAssignRequested(int layer, int key, const QString& actionId) {
    try {
        if (const auto actionInfo = mAvailableActions.getAction(actionId); actionInfo != std::nullopt) {
            //TODO: update this so that profile data is saved more granularly
            // to not dump the entire thing just for some fields of it
            mCurrentProfile.layers[layer].actions[key] = *actionInfo;
            mService->saveProfile(mCurrentProfile);

            mKeypadController->onActionAssigned(layer, key, *actionInfo);
        }
    } catch(...) {
        qWarning() << "An error ocurred accessing the action to be assigned:" << layer << key << actionId;
    }
}

void KeypadModule::onKeySelected(int layer, int key) {
    try {
        const auto& action = mCurrentProfile.layers[layer].actions[key];
        mActionConfigController->onActionConfigChanged(layer, key, action);
    } catch(...) {
        qWarning() << "An error ocurred accessing the configuration of the selected key: " << layer << key;
    }
}

void KeypadModule::onKeyTriggered(int layer, int key) {
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
                utils::json::WriteVariant(actionPayload, configEntry.name, configEntry.type, configEntry.value);
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

void KeypadModule::onConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value) {
    try {
        auto& action = mCurrentProfile.layers[layer].actions[key];
        if (auto config = std::find_if(action.configs.begin(), action.configs.end(), [name](const auto& config) { return config.name == name; });
            config != action.configs.end())
        {
            config->value = value;
            mService->saveProfile(mCurrentProfile);

            mActionConfigController->onActionConfigOptionChanged(layer, key, name, value);
        }
    } catch(...) {
        qWarning() << "An error ocurred updating the key config option:" << layer << key << name;
    }
}
