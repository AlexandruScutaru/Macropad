#pragma once

#include "Types.h"

#include <QObject>
#include <QPointer>

#include <unordered_map>


namespace hid {
    class Device;
    class CommunicationThread;

    class DeviceService: public QObject {
        Q_OBJECT
    public:
        explicit DeviceService(QObject* parent = nullptr);
        explicit DeviceService(bool skipDeviceConnection, QObject* parent = nullptr);
        ~DeviceService();

        void connectToDevice();
        void registerMessageHandler(MessageType type, const MessageHandlerCb& handler);

    signals:
        void deviceConnected();
        void deviceNotFound();

    private slots:
        void onDeviceConnected();
        void onMessageReceived(const Message& message);
        void onReadError();

    private:
        void startReading();

        QPointer<hid::Device> mDevice{ nullptr };
        QPointer<CommunicationThread> mCommunicationThread{ nullptr };
        
        std::unordered_map<MessageType, MessageHandlerCb> mMessageHandlers;
        bool mSkipDeviceConnection{ false };

    };
}
