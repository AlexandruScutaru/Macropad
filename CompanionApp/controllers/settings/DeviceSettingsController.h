#pragma once

#include "AppSettings.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>
#include <QString>


class DeviceSettingsController: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString devicePath READ getDevicePath NOTIFY devicePathChanged)
    QML_ELEMENT
public:
    explicit DeviceSettingsController(QObject *parent = nullptr);
    ~DeviceSettingsController();

    Q_INVOKABLE void init(AppSettings* appSettings);
    Q_INVOKABLE void forgetDeviceClicked();

    QString getDevicePath();

    void setDevicePath(QString devicePath);

signals:
    void devicePathChanged(QString devicePath);

private:
    QPointer<AppSettings> mAppSettings{ nullptr };
    QString mDevicePath{ "device/path" };

};
