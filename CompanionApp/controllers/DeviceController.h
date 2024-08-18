#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

extern "C" {
#include <hidapi.h>
}

#include "models/DeviceInfoModel.h"

Q_DECLARE_OPAQUE_POINTER(hid_device*)

class HidHelper;
class PotentiometersReader;
class Config;


class DeviceController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit DeviceController(PotentiometersReader* potentiometersReader, Config* config, QObject* parent = nullptr);
    ~DeviceController();

    Q_INVOKABLE DeviceInfoModel* getDeviceInfoModel();
    Q_INVOKABLE Config* getConfig();
    Q_INVOKABLE void setIsCalibrating(bool isCalibrating);
    Q_INVOKABLE void search(const QString& vid, const QString& pid);
    Q_INVOKABLE void openDevice(const QString& path);
    Q_INVOKABLE void openLastDevice();

signals:
    void deviceOpened(hid_device* device);
    void deviceConnected();
    void noDeviceSaved();
    void potentiometersChanged(const std::vector<int>& values);
    void slidersChanged(const std::vector<int>& values);

private slots:
    void onPotentiometersChanged(const std::vector<int>& values);

private:
    static int convertToInt(const QString& hexStr);

    QPointer<Config> mConfig{ nullptr };
    QPointer<DeviceInfoModel> mDeviceInfoModel{ nullptr };
    QPointer<PotentiometersReader> mPotentiometersReader{ nullptr };
    QPointer<HidHelper> mHidHelper{ nullptr };

    bool mHidInitted{ false };
    bool mIsCalibrating{ false };

};
