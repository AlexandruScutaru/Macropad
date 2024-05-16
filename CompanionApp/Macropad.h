#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>


enum class HotKeys;

class AudioOutputSwitcher;
class DeviceController;
class PotentiometersReader;
class TrayIcon;

class Macropad : public QObject {
    Q_OBJECT
public:
    explicit Macropad(QQmlApplicationEngine& engine, QObject* parent = nullptr);
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
    void onHotKeyTriggered(HotKeys hotKey);

private:
    QObject* const getMainWindowObject();

    void initTrayIcon();
    void initDevHelperView(const QObject* const qmlWindow);
    void initDeviceView(const QObject* const qmlWindow);
    void initHotkey();

    QQmlApplicationEngine& mQmlEngine;

    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };
    PotentiometersReader* mPotentiometersReader{ nullptr };
    DeviceController* mDeviceController{nullptr};

    TrayIcon* mTrayIcon{ nullptr };

};
