#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

extern "C" {
#include <hidapi.h>
}

#include "DeviceInfoModel.h"
#include "DeviceCalibrationModel.h"

Q_DECLARE_OPAQUE_POINTER(hid_device*)

class HidHelper;
class PotentiometersReader;

struct PotentiometerInfo {
    int min{ 0 };
    int max{ 1023 };
    int value{ 0 };
};

class DeviceController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")

public:
    explicit DeviceController(PotentiometersReader* potentiometersReader, QObject* parent = nullptr);
    ~DeviceController();

    Q_INVOKABLE DeviceInfoModel* getDeviceInfoModel();
    Q_INVOKABLE DeviceCalibrationModel* getDeviceCalibrationModel();

    Q_INVOKABLE void search(const QString& vid, const QString& pid);
    Q_INVOKABLE void openDevice(const QString& path);
    Q_INVOKABLE void setIsCalibrating(bool isCalibrating);

signals:
    void deviceOpened(hid_device* device);
    void deviceConnected();
    void slidersChanged(const std::vector<int>& values);

private slots:
    void onPotentiometersUpdated(const std::vector<int>& values);

private:
    void handleCalibration(const std::vector<int>& rawValues);

    static int convertToInt(const QString& hexStr);

    std::vector<PotentiometerInfo> mPotentiometersInfo;

    QPointer<DeviceInfoModel> mDeviceInfoModel{ nullptr };
    QPointer<DeviceCalibrationModel> mDeviceCalibrationModel{ nullptr };

    QPointer<PotentiometersReader> mPotentiometersReader{ nullptr };
    HidHelper* mHidHelper{ nullptr };
    bool mHidInitted{ false };
    bool mIsCalibrating{ false };

};
