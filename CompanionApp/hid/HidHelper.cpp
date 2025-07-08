#include "HidHelper.h"

#include <QDebug>

#include <string>


static auto getHidError() {
    return QString::fromWCharArray(hid_error(nullptr));
}


HidHelper::HidHelper(QObject* parent)
    : QObject(parent)
{
    qDebug() << "HidHelper::HidHelper";
}

HidHelper::~HidHelper() {
    qDebug() << "HidHelper::~HidHelper";

    if (mDevice) {
        hid_close(mDevice);
    }

    mDevice = nullptr;
    hid_exit();
}

bool HidHelper::init() {
    if (hid_init() < 0) {
        qWarning() << "hid_init error: " << getHidError();
        return false;
    }

    return true;
}

std::vector<HidDeviceInfo> HidHelper::enumerateDevices(int vid, int pid) {
    std::vector<HidDeviceInfo> devicesInfo;

    const auto devices = hid_enumerate(vid, pid);
    auto device = devices;
    while(device) {
        devicesInfo.push_back({
            .vid = device->vendor_id,
            .pid = device->product_id,
            .usagePage = device->usage_page,
            .usageId = device->usage,
            .product = std::wstring(device->product_string),
            .manufacturer = std::wstring(device->manufacturer_string),
            .serial = std::wstring(device->serial_number),
            .path = std::string(device->path)
        });

        device = device->next;
        mDevicesCount++;
    }

    hid_free_enumeration(devices);

    return devicesInfo;
}

hid_device* HidHelper::openDevice(const std::string& path) {
    qDebug() << "Open device path: " << path;

    mDevice = hid_open_path(path.c_str());
     if (!mDevice) {
         qWarning() << "Unable to open HID device! error: " << getHidError();
         return nullptr;
    }

    hid_set_nonblocking(mDevice, 0);

    return mDevice;
}
