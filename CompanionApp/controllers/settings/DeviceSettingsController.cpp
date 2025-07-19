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
    emit devicePathChanged(mDevicePath);
}

void DeviceSettingsController::forgetDeviceClicked() {}

QString DeviceSettingsController::getDevicePath() {
    return mDevicePath;
}
