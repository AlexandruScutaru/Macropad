#pragma once

#include "Device.h"

#include <QPointer>
#include <QThread>


namespace hid {
    class HidCommunicationThread : public QThread {
        Q_OBJECT
        void run() override;
    public:
        explicit HidCommunicationThread(Device* device, QObject* parent = nullptr);
        ~HidCommunicationThread();

    signals:
        void messageReceived(const std::vector<unsigned char>& data);
        void readError();

    private:
        QPointer<Device> mDevice{ nullptr };

    };
}
