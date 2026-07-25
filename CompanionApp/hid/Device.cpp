#include "Device.h"

#include <QDebug>
#include <QtConcurrent>

#include <string>

extern "C" {
#include <hidapi.h>
}

using namespace hid;


Device::Device(QObject* parent)
    : QObject(parent)
{
    qDebug() << "Device::Device";
}

Device::~Device() {
    qDebug() << "Device::~Device";

    if (mDevice) {
        hid_close(mDevice);
    }

    mDevice = nullptr;
    hid_exit();
}


void Device::connect(int vid, int pid, int usagePage, int usageId) {
    if (mDeviceOpenFuture.isRunning()) {
        return;
    }

    if (!mInitialized && hid_init() < 0) {
        qWarning() << "hid_init error: " << GetHidError();
        return;
    }

    mInitialized = true;

    // I seem to get a memory leak around QFutureWatcher when calling this method multiple times
    // so I use a QSharedPointer to manage the lifetime of the watcher
    mDeviceOpenFutureWatcher = QSharedPointer<QFutureWatcher<hid_device*>>::create();

    QObject::connect(mDeviceOpenFutureWatcher.data(), &QFutureWatcher<hid_device*>::finished, this, [this, pThis = QPointer(this)]() {
        if (!pThis) {
            return;
        }

        mDevice = mDeviceOpenFuture.result();
        if (!mDevice) {
            emit deviceNotFound();
            return;
        }

        emit deviceConnected();
    });

    mDeviceOpenFuture = QtConcurrent::run([](int pid, int vid, int usagePage, int usageId) -> hid_device* {
        std::string devicePath = "";

        const auto devices = hid_enumerate(vid, pid);
        auto device = devices;
        while (device) {
            if (device->usage_page == usagePage && device->usage == usageId) {
                devicePath = std::string(device->path);
                break;
            }
            device = device->next;
        }

        hid_free_enumeration(devices);

        if (devicePath.empty()) {
            return static_cast<hid_device*>(nullptr);
        }

        hid_device* hidDevice = hid_open_path(devicePath.c_str());
        if (!hidDevice) {
            return static_cast<hid_device*>(nullptr);
        }

        hid_set_nonblocking(hidDevice, 0);

        return hidDevice;
    }, pid, vid, usagePage, usageId);

    mDeviceOpenFutureWatcher->setFuture(mDeviceOpenFuture);
}

QString Device::GetHidError() {
    return QString::fromWCharArray(hid_error(nullptr));
}
