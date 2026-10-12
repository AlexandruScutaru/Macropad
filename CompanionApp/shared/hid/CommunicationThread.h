#pragma once

#include "Device.h"

#include <QPointer>
#include <QThread>


namespace hid {
    class CommunicationThread : public QThread {
        Q_OBJECT
        void run() override;
    public:
        explicit CommunicationThread(Device* device, QObject* parent = nullptr);
        ~CommunicationThread();

    signals:
        void messageReceived(const Message& message);
        void readError();

    private:
        QPointer<Device> mDevice{ nullptr };

    };
}
