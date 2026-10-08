#pragma once

#include "context/IAppContext.h"
#include "theming/Theme.h"

#include <QApplication>
#include <QQmlApplicationEngine>


class Config;
class TrayIcon;

class App : public QApplication {
    Q_OBJECT
public:
    App(int& argc, char** argv);
    ~App();

signals:
    void showWindowRequested();

public slots:
    void onChangeThemeRequested(const QString& name);

private:
    CmdArgs parseCmdArgs(int& argc, char** argv);
    void initQmlEngine(const CmdArgs& args);
    void initTrayIcon();

    theme::Theme mTheme;
    QQmlApplicationEngine mQmlEngine;

    TrayIcon* mTrayIcon{ nullptr };
    IAppContext* mAppContext{ nullptr };
};

#define APP (qobject_cast<App*>(qApp))
