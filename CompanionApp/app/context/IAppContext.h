#pragma once

#include <QObject>


struct AppConfig {
    bool isDebug = false;
    bool isSkipPhysicalDevice = false;
    bool isPlayground = false;
};

namespace settings {
    class SettingsService;
}

namespace hid {
    class DeviceService;
}

class KeypadService; 

class IAppContext: public QObject {
    Q_OBJECT
public:
    explicit IAppContext(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IAppContext() = default;
    
    virtual AppConfig getAppConfig() const = 0;

    virtual settings::SettingsService* settingsService() = 0;
    virtual hid::DeviceService* hidDeviceService() = 0;
    virtual KeypadService* keypadService() = 0;

};
