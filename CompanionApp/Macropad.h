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

    Q_INVOKABLE QSize windowSize();
    Q_INVOKABLE void saveWindowSize(int w, int h);
    Q_INVOKABLE bool navBarExpanded();
    Q_INVOKABLE void saveNavBarExpanded(bool expanded);

signals:
    void showWindowRequested();
    void themeChanged(Theme* theme);

private:
    QObject* const getMainWindowObject();
    void loadTheme(ThemeVariant variant);
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
