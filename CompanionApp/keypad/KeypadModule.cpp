#include "KeypadModule.h"
#include "KeypadTypes.h"
#include "controller/KeypadController.h"
#include "service/KeypadService.h"
#include "AppSettings.h"

#include <QDebug>

#include <algorithm>

using namespace Keypad;


KeypadModule::KeypadModule(QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
{
    qDebug() << "KeypadModule::KeypadModule";

    const auto service = new KeypadService(appSettings, this);

    mActionConfigController = new ActionConfigController(this);
    mAvailableActionsController = new AvailableActionsController(service->getAvailableActions(), this);
    mKeypadController = new KeypadController(this);

    QObject::connect(service, &KeypadService::profileLoaded, mKeypadController, &KeypadController::onProfileChanged);
    QObject::connect(service, &KeypadService::actionAssigned, mKeypadController, &KeypadController::onActionAssigned);
    QObject::connect(service, &KeypadService::actionConfigChanged, mActionConfigController, &ActionConfigController::onActionConfigChanged);

    QObject::connect(mKeypadController, &KeypadController::actionAssignRequested, service, &KeypadService::onActionAssignRequested);
    QObject::connect(mKeypadController, &KeypadController::keySelected, service, &KeypadService::onKeySelected);
    QObject::connect(mKeypadController, &KeypadController::keyTriggered, service, &KeypadService::onKeyTriggered);

    QObject::connect(mActionConfigController, &ActionConfigController::configOptionChanged, service, &KeypadService::onConfigOptionChanged);

    QObject::connect(service, &KeypadService::actionTriggered, this, [this](const Keypad::Action& action) {
        qDebug() << "Action:" << action.name << "triggered!";
    });

    // will either get it from local cache or make an async request to get it from cloud if needed
    service->loadSavedProfile();
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
