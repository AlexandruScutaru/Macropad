#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QSize>


namespace hid {
    class Device;

    class DeviceService: public QObject {
        Q_OBJECT
    public:
        explicit DeviceService(QObject* parent = nullptr);
        explicit DeviceService(bool skipDeviceConnection, QObject* parent = nullptr);
        ~DeviceService();

        void connectToDevice();

    signals:
        void deviceConnected();
        void deviceNotFound();

    private:
        hid::Device* mHidDevice{ nullptr };
        bool mSkipDeviceConnection{ false };

    };
}
