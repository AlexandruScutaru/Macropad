#pragma once

#include <QApplication>
#include <QObject>
#include <QQmlApplicationEngine>


enum class HotKeys;

class TrayIcon;
class AudioOutputSwitcher;

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
    TrayIcon* mTrayIcon{ nullptr };
    AudioOutputSwitcher* mAudioOutputSwitcher{ nullptr };

};
