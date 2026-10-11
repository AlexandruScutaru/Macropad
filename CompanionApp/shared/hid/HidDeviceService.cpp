#include "HidDeviceService.h"
#include "Device.h"
#include "HidCommunicationThread.h"

#include <QDebug>

#include <iostream>


static constexpr auto VID = 0xFEED;
static constexpr auto PID = 0xB00B;
static constexpr auto USAGE_PAGE = 0xFF60;
static constexpr auto USAGE_ID = 0x61;


using namespace hid;

DeviceService::DeviceService(QObject* parent)
    : QObject(parent)
{
    qDebug() << "DeviceService::DeviceService";

    mHidDevice = new hid::Device(this);
    QObject::connect(mHidDevice, &hid::Device::deviceConnected, this, &DeviceService::onDeviceConnected);
    QObject::connect(mHidDevice, &hid::Device::deviceNotFound, this, &DeviceService::deviceNotFound);
}

DeviceService::DeviceService(bool skipDeviceConnection, QObject* parent)
    : DeviceService(parent)
{
    mSkipDeviceConnection = skipDeviceConnection;
}

DeviceService::~DeviceService() {
    qDebug() << "DeviceService::~DeviceService";

    if (mHidCommunicationThread && mHidCommunicationThread->isRunning()) {
        mHidCommunicationThread->requestInterruption();
        mHidCommunicationThread->wait();
    }
}


void DeviceService::connectToDevice() {
    if (mSkipDeviceConnection) {
        emit deviceConnected();
        return;
    }

    if (mHidDevice) {
        mHidDevice->connect(VID, PID, USAGE_PAGE, USAGE_ID);
    }
}

void DeviceService::onDeviceConnected() {
    emit deviceConnected();

    startReading();
}

void DeviceService::onMessageReceived(const std::vector<unsigned char>& data) {
    assert(data.size() == 32 && "Expected exactly 32 bytes payload");

    std::printf("received message: ");

    for (const auto& c: data) {
        std::printf("%d ", c);
    }

    std::printf("\n");

    decodeMessage(data);
}

void DeviceService::onReadError() {
    qWarning() << "hid device read error, probably the device was disconnected";
    emit deviceNotFound();
}


void DeviceService::startReading() {
    mHidCommunicationThread = new HidCommunicationThread(mHidDevice, this);
    QObject::connect(mHidCommunicationThread, &HidCommunicationThread::messageReceived, this, &DeviceService::onMessageReceived);
    QObject::connect(mHidCommunicationThread, &HidCommunicationThread::readError, this, &DeviceService::onReadError);
    QObject::connect(mHidCommunicationThread, &HidCommunicationThread::finished, mHidCommunicationThread, &QObject::deleteLater);
    mHidCommunicationThread->start();
}

void DeviceService::decodeMessage(const std::vector<unsigned char>& data) {
    if (data[0] == 1) {
        auto key = data[1] * 3 + data[2];
        emit keyPressed(0, key);
    }
}
