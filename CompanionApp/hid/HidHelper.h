#pragma once

#include <QObject>

#include <vector>

extern "C" {
#include <hidapi.h>
}


struct HidDeviceInfo {
    int vid{ 0 };
    int pid{ 0 };
    int usagePage{ 0 };
    int usageId{ 0 };
    std::wstring product;
    std::wstring manufacturer;
    std::wstring serial;
    std::string path;
};


class HidHelper : public QObject {
    Q_OBJECT
public:
    explicit HidHelper(QObject* parent = nullptr);
    ~HidHelper();

    bool init();
    std::vector<HidDeviceInfo> enumerateDevices(int vid, int pid);
    hid_device* openDevice(const std::string& path);


private:
    int mDevicesCount{ 0 };
    hid_device* mDevice{ nullptr };

};
