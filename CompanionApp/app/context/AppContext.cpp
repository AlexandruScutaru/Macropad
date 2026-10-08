#include "AppContext.h"
#include "app/Config.h"
#include "app/App.h"
#include "modules/keypad/service/KeypadService.h"
#include "shared/hid/HidDeviceService.h"
#include "modules/settings/service/SettingsService.h"
#include "shared/actions/system/SystemActions.h"
#include "shared/os/IPlatform.h"

#include <QDebug>


AppContext::AppContext(Config* config, const CmdArgs& args, QObject *parent)
    : IAppContext(parent)
    , mArgs(args)
{
    qDebug() << "AppContext::AppContext";

    mSettingsService = new settings::SettingsService(config, this);
    QObject::connect(APP, &App::showWindowRequested, mSettingsService, &settings::SettingsService::showWindowRequested);
    QObject::connect(mSettingsService, &settings::SettingsService::changeThemeRequested, APP, &App::onChangeThemeRequested);
    APP->onChangeThemeRequested(mSettingsService->getCurrentTheme());
    
    mHidDeviceService = new hid::DeviceService(args.isSkipPhysicalDevice, this);    
    mKeypadService = new KeypadService(config, this);

    initActionHandlers();
}

AppContext::~AppContext() {
    qDebug() << "AppContext::~AppContext";
}


CmdArgs AppContext::getCmdArgs() const {
    return mArgs;
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
