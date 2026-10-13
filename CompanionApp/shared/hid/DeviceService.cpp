#include "DeviceService.h"
#include "Device.h"
#include "CommunicationThread.h"
#include "shared/hid/Types.h"

#include <QDebug>


static constexpr auto VID = 0xFEED;
static constexpr auto PID = 0xB00B;
static constexpr auto USAGE_PAGE = 0xFF60;
static constexpr auto USAGE_ID = 0x61;


using namespace hid;

DeviceService::DeviceService(QObject* parent)
    : QObject(parent)
{
    qDebug() << "DeviceService::DeviceService";

    mDevice = new hid::Device(this);
    QObject::connect(mDevice, &hid::Device::deviceConnected, this, &DeviceService::onDeviceConnected);
    QObject::connect(mDevice, &hid::Device::deviceNotFound, this, &DeviceService::deviceNotFound);
}

DeviceService::DeviceService(bool skipDeviceConnection, QObject* parent)
    : DeviceService(parent)
{
    mSkipDeviceConnection = skipDeviceConnection;
}

DeviceService::~DeviceService() {
    qDebug() << "DeviceService::~DeviceService";

    if (mCommunicationThread && mCommunicationThread->isRunning()) {
        mCommunicationThread->requestInterruption();
        mCommunicationThread->wait();
    }
}


void DeviceService::connectToDevice() {
    if (mSkipDeviceConnection) {
        emit deviceConnected();
        return;
    }

    if (mDevice) {
        mDevice->connect(VID, PID, USAGE_PAGE, USAGE_ID);
    }
}

void DeviceService::registerMessageHandler(MessageType type, const MessageHandlerCb& handler) {
    mMessageHandlers[type] = handler;
}

void DeviceService::onDeviceConnected() {
    emit deviceConnected();

    startReading();
}

void DeviceService::onMessageReceived(const Message& message) {
    auto messageType = static_cast<MessageType>(message[0]);

    // this takes precedence, no other component would need to handle main device state changes
    if (messageType == MessageType::SlidersModuleState) {
        emit slidersModuleStateChanged(message[1] == 1);
        return;
    }

    if (const auto& it = mMessageHandlers.find(messageType); it != mMessageHandlers.end()) {
        it->second(message);
    }
}

void DeviceService::onReadError() {
    qWarning() << "hid device read error, probably the device was disconnected";
    emit deviceNotFound();
}

void DeviceService::startReading() {
    mCommunicationThread = new CommunicationThread(mDevice, this);
    QObject::connect(mCommunicationThread, &CommunicationThread::messageReceived, this, &DeviceService::onMessageReceived);
    QObject::connect(mCommunicationThread, &CommunicationThread::readError, this, &DeviceService::onReadError);
    QObject::connect(mCommunicationThread, &CommunicationThread::finished, mCommunicationThread, &QObject::deleteLater);
    mCommunicationThread->start();
}
