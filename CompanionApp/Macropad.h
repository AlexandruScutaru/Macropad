#pragma once

#include "theming/Theme.h"

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QPointer>

#include <memory>
#include <vector>


class AppSettings;
class KeypadModule;
class TrayIcon;

class IActionHandler;
using IActionHandlerPtr = std::shared_ptr<IActionHandler>;

namespace hid {
    class Device;
}

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

    void init(const MacropadConfig& config);
    theme::Theme* getTheme();

    Q_INVOKABLE void connectToDevice();
    Q_INVOKABLE KeypadModule* getKeypadModule();

    Q_INVOKABLE QSize windowSize();
    Q_INVOKABLE void saveWindowSize(int w, int h);
    Q_INVOKABLE bool navBarExpanded();
    Q_INVOKABLE void saveNavBarExpanded(bool expanded);

signals:
    void showWindowRequested();
    void themeChanged(theme::Theme* theme);
    void deviceConnected();
    void deviceNotFound();

private:
    QObject* const getMainWindowObject();
    void loadTheme(theme::Type type);

    void initActionHandlers();
    void initTrayIcon();
    void initAppStackView(const QObject* const qmlWindow);

    QQmlApplicationEngine& mQmlEngine;

    MacropadConfig mConfig;
    AppSettings* mAppSettings{ nullptr };
    KeypadModule* mKeypadModule{ nullptr };
    hid::Device* mHidDevice{ nullptr };
    TrayIcon* mTrayIcon{ nullptr };

    QPointer<theme::Theme> mTheme{ nullptr };

    std::vector<IActionHandlerPtr> mActionHandlers;

};
