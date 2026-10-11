#pragma once

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QSharedPointer>


struct hid_device_;
typedef struct hid_device_ hid_device;

namespace hid {
    // maybe move to a newer standard std::expected
    struct ReadResult {
        std::vector<unsigned char> data;
        bool error{ false };
    };

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
