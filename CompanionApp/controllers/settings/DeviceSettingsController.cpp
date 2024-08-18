#include "controllers/settings/DeviceSettingsController.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>


DeviceSettingsController::DeviceSettingsController(QObject *parent)
    : QObject(parent)
{
    qDebug() << "DeviceSettingsController::DeviceSettingsController";
}

DeviceSettingsController::~DeviceSettingsController() {
    qDebug() << "DeviceSettingsController::~DeviceSettingsController";
}


void DeviceSettingsController::init(Config* config) {
    mConfig = config;
    mDevicePath = mConfig->lastDevicePath();
    emit devicePathChanged(mDevicePath);
}

void DeviceSettingsController::forgetDeviceClicked() {
    mConfig->clearLastDevicePath();
    mDevicePath = mConfig->lastDevicePath();
    emit devicePathChanged(mDevicePath);
}

QString DeviceSettingsController::getDevicePath() {
    return mDevicePath;
}
