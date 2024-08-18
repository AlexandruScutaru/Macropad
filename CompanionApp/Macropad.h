#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>

namespace Hotkeys {
    enum class Actions;
}

class AudioOutputSwitcher;
class Config;
class DeviceController;
class PotentiometersReader;
class TrayIcon;


class Macropad : public QObject {
    Q_OBJECT
public:
    explicit Macropad(QQmlApplicationEngine& engine, Config* config, QObject* parent = nullptr);
    ~Macropad();

    void onInitialized(bool isDebug);

signals:
    void showWindowRequested();

public slots:
    bool devHelperExpandState();
    void saveDevHelperExpandState(bool isExpanded);
    QSize windowSize();
    void saveWindowSize(int w, int h);

private slots:
    void onHotkeyTriggered(int key);

private:
    QObject* const getMainWindowObject();

    void initTrayIcon();
    void initDevHelperView(const QObject* const qmlWindow);
    void initDeviceView(const QObject* const qmlWindow);
    void initHotkeysFilter();

    QQmlApplicationEngine& mQmlEngine;

    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };
    Config* mConfig{ nullptr };
    PotentiometersReader* mPotentiometersReader{ nullptr };
    DeviceController* mDeviceController{nullptr};

    TrayIcon* mTrayIcon{ nullptr };

};
