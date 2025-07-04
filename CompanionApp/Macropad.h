#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>

namespace Hotkeys {
    enum class Actions;
}

class AudioOutputSwitcher;
class AppSettings;
class MainController;
class PotentiometersReader;
class TrayIcon;

struct MacropadConfig {
    bool isDebug = false;
    bool isSkipPhysicalDevice = false;
};

class Macropad : public QObject {
    Q_OBJECT
public:
    explicit Macropad(QQmlApplicationEngine& engine, AppSettings* appSettings, QObject* parent = nullptr);
    ~Macropad();

    void onInitialized(const MacropadConfig& config);

signals:
    void showWindowRequested();

public slots:
    QSize windowSize();
    void saveWindowSize(int w, int h);

private slots:
    void onHotkeyTriggered(int key);

private:
    QObject* const getMainWindowObject();

    void initTrayIcon();
    void initDeviceView(const QObject* const qmlWindow);
    void initHotkeysFilter();

    QQmlApplicationEngine& mQmlEngine;

    MacropadConfig mConfig;
    AppSettings* mAppSettings{ nullptr };
    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };
    PotentiometersReader* mPotentiometersReader{ nullptr };
    MainController* mMainController{nullptr};

    TrayIcon* mTrayIcon{ nullptr };

};
