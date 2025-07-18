#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QPointer>

#include "theming/Theme.h"

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
    bool isPlayground = false;
};


class Macropad : public QObject {
    Q_OBJECT
public:
    explicit Macropad(QQmlApplicationEngine& engine, AppSettings* appSettings, QObject* parent = nullptr);
    ~Macropad();

    void onInitialized(const MacropadConfig& config);
    Theme* getTheme();

signals:
    void showWindowRequested();
    void themeChanged(Theme* theme);

public slots:
    QSize windowSize();
    void saveWindowSize(int w, int h);
    void loadTheme(ThemeVariant variant);

private:
    QObject* const getMainWindowObject();

    void initTrayIcon();
    void initAppStackView(const QObject* const qmlWindow);

    QQmlApplicationEngine& mQmlEngine;

    MacropadConfig mConfig;
    QPointer<Theme> mTheme{ nullptr };
    AppSettings* mAppSettings{ nullptr };
    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };
    PotentiometersReader* mPotentiometersReader{ nullptr };
    MainController* mMainController{nullptr};

    TrayIcon* mTrayIcon{ nullptr };

};
