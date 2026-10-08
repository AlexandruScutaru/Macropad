#pragma once

#include "IAppContext.h"

#include <memory>
#include <vector>


class Config;

namespace action_handlers {
    class IActionHandler;
    using IActionHandlerPtr = std::shared_ptr<IActionHandler>;
}

class AppContext: public IAppContext {
    Q_OBJECT
public:
    AppContext(Config* config, const CmdArgs& args, QObject *parent = nullptr);
    ~AppContext();

    CmdArgs getCmdArgs() const override;

    settings::SettingsService* settingsService() override;
    hid::DeviceService* hidDeviceService() override;
    KeypadService* keypadService() override;

private:
    void initActionHandlers();

    CmdArgs mArgs;
    std::vector<action_handlers::IActionHandlerPtr> mActionHandlers;

    settings::SettingsService* mSettingsService{ nullptr };
    hid::DeviceService* mHidDeviceService{ nullptr };
    KeypadService* mKeypadService{ nullptr };


};
