#include "HidCommunicationThread.h"
#include "Device.h"

#include <QDebug>


using namespace hid;

HidCommunicationThread::HidCommunicationThread(Device* device, QObject* parent)
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
        auto result = mDevice->read();

        if (result.error) {
            emit readError();
            return;
        }

        if (result.data.size() > 0) {
            emit messageReceived(result.data);
        }
    }
}
