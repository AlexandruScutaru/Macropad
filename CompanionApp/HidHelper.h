#pragma once

#include <QObject>

extern "C" {
#include <hidapi.h>
}


class HidHelper : public QObject {
    Q_OBJECT
public:
    explicit HidHelper(QObject* parent = nullptr);
    ~HidHelper();

    bool init();
    hid_device* openDevice(int vid, int pid);


private:
    int mDevicesCount{ 0 };
    hid_device* mDevice{ nullptr };

};
