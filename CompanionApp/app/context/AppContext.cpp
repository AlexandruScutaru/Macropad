#include "AppContext.h"
#include "app/AppSettings.h"
#include "app/App.h"
#include "module/keypad/service/KeypadService.h"
#include "services/HidDeviceService.h"
#include "services/SettingsService.h"
#include "action-handlers/system/SystemActions.h"
#include "os/IPlatform.h"

#include <QDebug>


AppContext::AppContext(AppSettings* appSettings, const AppConfig& config, QObject *parent)
    : IAppContext(parent)
    , mConfig(config)
{
    qDebug() << "AppContext::AppContext";

    mSettingsService = new settings::SettingsService(appSettings, this);
    QObject::connect(APP, &App::showWindowRequested, mSettingsService, &settings::SettingsService::showWindowRequested);
    QObject::connect(mSettingsService, &settings::SettingsService::changeThemeRequested, APP, &App::onChangeThemeRequested);
    APP->onChangeThemeRequested(mSettingsService->getCurrentTheme());
    
    mHidDeviceService = new hid::DeviceService(config.isSkipPhysicalDevice, this);    
    mKeypadService = new KeypadService(appSettings, this);

    initActionHandlers();
}

AppContext::~AppContext() {
    qDebug() << "AppContext::~AppContext";
}


AppConfig AppContext::getAppConfig() const {
    return mConfig;
}

settings::SettingsService* AppContext::settingsService() {
    assert(mSettingsService && "SettingsService is null");
    return mSettingsService;
}

hid::DeviceService* AppContext::hidDeviceService() {
    assert(mHidDeviceService && "HidDeviceService is null");
    return mHidDeviceService;
}

KeypadService* AppContext::keypadService() {
    assert(mKeypadService && "KeypadService is null");
    return mKeypadService;
}


void AppContext::initActionHandlers() {
    auto platform = os::CreatePlatform();

    mActionHandlers.push_back(std::make_shared<action_handlers::SystemActions>(platform));

    for (const auto& handler: mActionHandlers) {
        mKeypadService->registerHandler(handler);
    }
}
