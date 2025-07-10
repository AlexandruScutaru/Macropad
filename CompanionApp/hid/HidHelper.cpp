#include "HidHelper.h"

#include <QDebug>
#include <QtConcurrent>

#include <string>


static auto getHidError() {
    return QString::fromWCharArray(hid_error(nullptr));
}


HidHelper::HidHelper(int vid, int pid, int usagePage, int usageId, QObject* parent)
    : QObject(parent)
    , mVid(vid), mPid(pid), mUsagePage(usagePage), mUsageId(usageId)
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

void HidHelper::openDevice() {
    if (mDeviceOpenFuture.isRunning()) {
        return;
    }

    // I seem to get a memory leak around QFutureWatcher when calling this method multiple times
    // so I use a QSharedPointer to manage the lifetime of the watcher
    mDeviceOpenFutureWatcher = QSharedPointer<QFutureWatcher<hid_device*>>::create();

    QObject::connect(mDeviceOpenFutureWatcher.data(), &QFutureWatcher<hid_device*>::finished, this, [this, pThis = QPointer(this)]() {
        if (!pThis) {
            return;
        }
        emit deviceOpened(mDeviceOpenFuture.result());
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
    }, mPid, mVid, mUsagePage, mUsageId);

    mDeviceOpenFutureWatcher->setFuture(mDeviceOpenFuture);
}
