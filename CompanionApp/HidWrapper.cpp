#include "HidWrapper.h"

#include <QDebug>
#include <atlstr.h>

#include <string>


static std::string getHidError() {
    return std::string(CW2A(hid_error(nullptr)));
}


HidWrapper::HidWrapper(QObject* parent)
    : QObject(parent)
{
    qDebug() << "HidWrapper::HidWrapper";
}

HidWrapper::~HidWrapper() {
    qDebug() << "HidWrapper::~HidWrapper";

    if (mDevice) {
        hid_close(mDevice);
    }

    mDevice = nullptr;
    hid_exit();
}

bool HidWrapper::init() {
    if (hid_init() < 0) {
        qWarning() << "hid_init error: " << getHidError();
        return false;
    }

    return true;
}

bool HidWrapper::openDevice(int vid, int pid) {
    std::string path;

    const auto devices = hid_enumerate(vid, pid);
    qDebug() << "Available devices:";
    auto device = devices;
    while(device) {
        if (device->usage_page == 0xFF60 && device->usage == 0x61) {
            path = std::string(device->path);
        }

        qDebug()
                << " vid: " << device->vendor_id
                << " pid: " << device->product_id
                << " serial: " << std::string(CW2A(device->serial_number))
                << " product: " << std::string(CW2A(device->product_string))
                << " usage page: " << device->usage_page
                << " usage id: " << device->usage
                << " path: " << device->path;

        device = device->next;
        mDevicesCount++;

    }

    hid_free_enumeration(devices);

    if (path.empty()) {
        qWarning() << "couldn't find device";
        return false;
    }

    qDebug() << "Trying to open path: " << path;

    mDevice = hid_open_path(path.c_str());
    if (!mDevice) {
        qWarning() << "Unable to open HID device! error: " << getHidError();
        return false;
    }

    hid_set_nonblocking(mDevice, 0);

    const int length = 128;
    wchar_t buffer[length];
    if (hid_get_product_string(mDevice, buffer, length) < 0) {
        qWarning() << "could not get HID device product string" << getHidError();
    } else {
        qDebug() << "opened device:  " << std::string(CW2A(buffer));
    }

    return true;
}

bool HidWrapper::recv() {
    unsigned char data[32] = {0};
    if (hid_read(mDevice, data, 32) < 0) {
        qWarning() << "error reading hid " << getHidError();
        return false;
    }

    auto pot = data[1];
    auto value = data[2] + (data[3] << 8);
    qDebug() << "HID reading: pot " << pot << " value " << value;

    return true;
}
