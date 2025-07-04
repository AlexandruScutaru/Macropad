#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

extern "C" {
#include <hidapi.h>
}

#include "models/DeviceInfoModel.h"
#include "models/SlidersModel.h"

Q_DECLARE_OPAQUE_POINTER(hid_device*)

class HidHelper;
class PotentiometersReader;
class AppSettings;


class MainController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit MainController(PotentiometersReader* potentiometersReader, AppSettings* appSettings, bool skipPhysicalDevice, QObject* parent = nullptr);
    ~MainController();

    Q_INVOKABLE AppSettings* getAppSettings();

    Q_INVOKABLE DeviceInfoModel* getDeviceInfoModel();
    Q_INVOKABLE SlidersModel* getSlidersModel();

    Q_INVOKABLE void search(const QString& vid, const QString& pid);
    Q_INVOKABLE void openDevice(const QString& path);
    Q_INVOKABLE void openLastDevice();

    Q_INVOKABLE void onSwitchOutputClicked();
    Q_INVOKABLE void onSliderMoved(int sliderId, int value);
    Q_INVOKABLE void setIsCalibrating(bool isCalibrating);

signals:
    void deviceOpened(hid_device* device);
    void deviceConnected();
    void noDeviceSaved();
    void potentiometersChanged(const std::vector<int>& values);
    void switchOutputRequested();

private slots:
    void onPotentiometersChanged(const std::vector<int>& values);

private:
    static int convertToInt(const QString& hexStr);

    QPointer<AppSettings> mAppSettings{ nullptr };
    QPointer<DeviceInfoModel> mDeviceInfoModel{ nullptr };
    QPointer<SlidersModel> mSlidersModel{ nullptr };
    QPointer<PotentiometersReader> mPotentiometersReader{ nullptr };
    QPointer<HidHelper> mHidHelper{ nullptr };

    bool mHidInitted{ false };
    bool mIsCalibrating{ false };
    bool mSkipPhysicalDevice{ false };

};
