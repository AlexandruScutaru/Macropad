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

struct PotentiometerInfo;
using Potentiometers =  std::vector<PotentiometerInfo>;


class MainController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit MainController(PotentiometersReader* potentiometersReader, const Potentiometers& potentiometersInfo, bool skipPhysicalDevice, QObject* parent = nullptr);
    ~MainController();

    Q_INVOKABLE DeviceInfoModel* getDeviceInfoModel();
    Q_INVOKABLE SlidersModel* getSlidersModel();

    Q_INVOKABLE void connectToDevice();
    Q_INVOKABLE void setIsCalibrating(bool isCalibrating);

    Q_INVOKABLE void onSwitchOutputClicked();
    Q_INVOKABLE void onSliderMoved(int sliderId, int value);

signals:
    void deviceConnected();
    void deviceNotFound();
    void potentiometersChanged(const std::vector<int>& values);
    void switchOutputRequested();

private slots:
    void onDeviceConnected(hid_device* device);
    void onPotentiometersChanged(const std::vector<int>& values);

private:
    QPointer<DeviceInfoModel> mDeviceInfoModel{ nullptr };
    QPointer<SlidersModel> mSlidersModel{ nullptr };
    QPointer<PotentiometersReader> mPotentiometersReader{ nullptr };
    QPointer<HidHelper> mHidHelper{ nullptr };

    Potentiometers mPotentiometersInfo;
    bool mHidInitted{ false };
    bool mIsCalibrating{ false };
    bool mSkipPhysicalDevice{ false };

};
