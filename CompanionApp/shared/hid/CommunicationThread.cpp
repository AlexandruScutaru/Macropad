#include "CommunicationThread.h"
#include "Device.h"

#include <QDebug>


using namespace hid;

CommunicationThread::CommunicationThread(Device* device, QObject* parent)
    : QThread(parent)
    , mDevice(device)
{
    qDebug() << "CommunicationThread::CommunicationThread";
}

CommunicationThread::~CommunicationThread() {
    qDebug() << "CommunicationThread::~CommunicationThread";
}


void CommunicationThread::run() {
    while (!isInterruptionRequested()) {
        auto result = mDevice->read();

        if (result.error) {
            emit readError();
            return;
        }

        // explicit check before the messages leave on to the receivers
        // the way the HID device communicates is via fixed 32 bytes payloads
        if (result.message.size() == hid::BUFFER_SIZE) {
            emit messageReceived(result.message);
        }
    }
}
