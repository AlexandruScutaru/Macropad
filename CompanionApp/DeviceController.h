#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

extern "C" {
#include <hidapi.h>
}

#include "DeviceInfoModel.h"


Q_DECLARE_OPAQUE_POINTER(hid_device*)

class HidHelper;

class DeviceController : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DeviceController(QObject* parent = nullptr);
    ~DeviceController();

    Q_INVOKABLE DeviceInfoModel* getModel();
    Q_INVOKABLE void search(const QString& vid, const QString& pid);
    Q_INVOKABLE void openDevice(const QString& path);

signals:
    void deviceOpened(hid_device* device);
    void deviceConnected();

private:
    static int convertToInt(const QString& hexStr);

    QPointer<DeviceInfoModel> mModel{ nullptr };
    HidHelper* mHidHelper{ nullptr };
    bool mHidInitted{ false };

};
