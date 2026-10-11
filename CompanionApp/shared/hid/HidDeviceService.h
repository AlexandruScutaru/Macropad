#pragma once

#include <QObject>
#include <QPointer>


namespace hid {
    class Device;
    class HidCommunicationThread;

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
        void keyPressed(int layer, int key);

    private slots:
        void onDeviceConnected();
        void onMessageReceived(const std::vector<unsigned char>& data);
        void onReadError();

    private:
        void startReading();
        void decodeMessage(const std::vector<unsigned char>& data);

        QPointer<hid::Device> mHidDevice{ nullptr };
        QPointer<HidCommunicationThread> mHidCommunicationThread{ nullptr };

        bool mSkipDeviceConnection{ false };

    };
}
