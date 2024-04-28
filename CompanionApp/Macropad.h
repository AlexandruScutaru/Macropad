#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>


enum class HotKeys;

class AudioOutputSwitcher;
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

private slots:
    void onHotKeyTriggered(HotKeys hotKey);

private:
    void initTrayIcon();
    void initDevHelperView();
    void initHotkey();

    QQmlApplicationEngine& mQmlEngine;
    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };
    PotentiometersReader* mPotentiometersReader{ nullptr };
    TrayIcon* mTrayIcon{ nullptr };

};
