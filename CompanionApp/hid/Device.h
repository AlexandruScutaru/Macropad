#pragma once

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QSharedPointer>

#include <vector>

struct hid_device_;
typedef struct hid_device_ hid_device;


namespace hid {
    class Device : public QObject {
        Q_OBJECT
    public:
        explicit Device(QObject* parent = nullptr);
        ~Device();

        void connect(int vid, int pid, int usagePage, int usageId);

    signals:
        void deviceConnected();
        void deviceNotFound();

    private:
        QFuture<hid_device*> mDeviceOpenFuture;
        QSharedPointer<QFutureWatcher<hid_device*>> mDeviceOpenFutureWatcher;

        hid_device* mDevice{ nullptr };
        bool mInitialized{ false };

        static QString GetHidError();

    };
}
