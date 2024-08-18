#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSize>

#include <memory>
#include <unordered_map>

class QSettings;

namespace Hotkeys {
    enum class Actions;
}

struct PotentiometerInfo {
    int min{ 0 };
    int max{ 1023 };
};

using HotkeyActionMap = std::unordered_map<Hotkeys::Actions, int>;
using Potentiometers =  std::vector<PotentiometerInfo>;


class Config : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit Config(QObject* parent = nullptr);
    ~Config();

    QSize windowSize();
    void saveWindowSize(const QSize& size);

    bool devHelperState();
    void saveDevHelperState(bool state);

    QString lastDevicePath();
    void saveLastDevicePath(const QString& path);
    void clearLastDevicePath();

    HotkeyActionMap hotkeyActionMap();
    void saveHotkeyActionMap(const HotkeyActionMap& hotkeyActionMap);
    Hotkeys::Actions keyToHotkeyAction(int key);

    Potentiometers potentiometersInfo();
    void savePotentiometersInfo(const Potentiometers& potentiometers);

private:
    std::unique_ptr<QSettings> getSettings();

    void readWindowState();
    void readDeviceConnectionData();
    void readHotkeyActions();
    void readPotentiometersInfo();

    QSize mWindowSize;
    bool mDevHelperState{ false };
    QString mLastDevicePath;
    HotkeyActionMap mHotkeyActionMap;
    Potentiometers mPotentiometersInfo;

};
