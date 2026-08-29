#pragma once

#include "IAppContext.h"

#include <memory>
#include <vector>


class AppSettings;

namespace action_handlers {
    class IActionHandler;
    using IActionHandlerPtr = std::shared_ptr<IActionHandler>;
}

class AppContext: public IAppContext {
    Q_OBJECT
public:
    AppContext(AppSettings* appSettings, const AppConfig& config, QObject *parent = nullptr);
    ~AppContext();

    AppConfig getAppConfig() const override;

    settings::SettingsService* settingsService() override;
    hid::DeviceService* hidDeviceService() override;
    KeypadService* keypadService() override;

private:
    void initActionHandlers();

    AppConfig mConfig;
    std::vector<action_handlers::IActionHandlerPtr> mActionHandlers;

    settings::SettingsService* mSettingsService{ nullptr };
    hid::DeviceService* mHidDeviceService{ nullptr };
    KeypadService* mKeypadService{ nullptr };


};
