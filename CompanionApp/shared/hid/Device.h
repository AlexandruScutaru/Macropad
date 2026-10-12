#pragma once

#include "Types.h"

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QSharedPointer>


struct hid_device_;
typedef struct hid_device_ hid_device;


namespace hid {
    inline constexpr size_t BUFFER_SIZE = 32;

    class Device : public QObject {
        Q_OBJECT
    public:
        explicit Device(QObject* parent = nullptr);
        ~Device();

        void connect(int vid, int pid, int usagePage, int usageId);

        // call is blocking, suitable to be called from a dedicated thread
        ReadResult read();

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
