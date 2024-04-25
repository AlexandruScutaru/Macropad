#pragma once

#include <QObject>

extern "C" {
#include <hidapi.h>
}


class HidWrapper : public QObject {
    Q_OBJECT
public:
    explicit HidWrapper(QObject* parent = nullptr);
    ~HidWrapper();

    bool init();
    bool openDevice(int vid, int pid);
    bool recv();

private:
    void processEvents();

    int mDevicesCount{ 0 };
    hid_device* mDevice{ nullptr };

};
