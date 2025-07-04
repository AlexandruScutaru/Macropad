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


void DeviceSettingsController::init(AppSettings* appSettings) {
    mAppSettings = appSettings;
    mDevicePath = mAppSettings->lastDevicePath();
    emit devicePathChanged(mDevicePath);
}

void DeviceSettingsController::forgetDeviceClicked() {
    mAppSettings->clearLastDevicePath();
    mDevicePath = mAppSettings->lastDevicePath();
    emit devicePathChanged(mDevicePath);
}

QString DeviceSettingsController::getDevicePath() {
    return mDevicePath;
}
