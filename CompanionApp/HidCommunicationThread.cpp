#include "HidCommunicationThread.h"

#include <QDebug>


HidCommunicationThread::HidCommunicationThread(hid_device* device, QObject* parent)
    : QThread(parent)
    , mDevice(device)
{
    qDebug() << "HidCommunicationThread::HidCommunicationThread";
}

HidCommunicationThread::~HidCommunicationThread() {
    qDebug() << "HidCommunicationThread::~HidCommunicationThread";
}


void HidCommunicationThread::run() {
    while (!isInterruptionRequested()) {
        std::memset(mBuffer, 0, BUFFER_LENGTH);
        auto res = hid_read_timeout(mDevice, mBuffer, BUFFER_LENGTH, 50);

        if (res < 0) {
            emit readError();
            return;
        }

        if (res > 0) {
            auto data = std::vector<unsigned char>(mBuffer, mBuffer + BUFFER_LENGTH);
            emit messageReceived(data);
        }
    }
}
