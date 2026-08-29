#include "HidDeviceService.h"
#include "hid/Device.h"

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

    mHidDevice = new hid::Device(this);
    QObject::connect(mHidDevice, &hid::Device::deviceConnected, this, &DeviceService::deviceConnected);
    QObject::connect(mHidDevice, &hid::Device::deviceNotFound, this, &DeviceService::deviceNotFound);
}

DeviceService::DeviceService(bool skipDeviceConnection, QObject* parent)
    : DeviceService(parent)
{
    mSkipDeviceConnection = skipDeviceConnection;
}

DeviceService::~DeviceService() {
    qDebug() << "DeviceService::~DeviceService";
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
