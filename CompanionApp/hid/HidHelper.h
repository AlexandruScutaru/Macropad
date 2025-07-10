#pragma once

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QSharedPointer>

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
    explicit HidHelper(int vid, int pid, int usagePage, int usageId, QObject* parent = nullptr);
    ~HidHelper();

    bool init();
    void openDevice();

signals:
    void deviceOpened(hid_device* device);

private:
    int mVid{ 0 };
    int mPid{ 0 };
    int mUsagePage{ 0 };
    int mUsageId{ 0 };

    QFuture<hid_device*> mDeviceOpenFuture;
    QSharedPointer<QFutureWatcher<hid_device*>> mDeviceOpenFutureWatcher;

    hid_device* mDevice{ nullptr };

};
