#pragma once

#include "context/IAppContext.h"
#include "theming/Theme.h"

#include <QApplication>
#include <QQmlApplicationEngine>


class AppSettings;
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
    AppConfig getConfig(int& argc, char** argv);
    void initQmlEngine(const AppConfig& config);
    void initTrayIcon();

    theme::Theme* getTheme();

    QQmlApplicationEngine mQmlEngine;

    TrayIcon* mTrayIcon{ nullptr };
    IAppContext* mAppContext{ nullptr };
    QPointer<theme::Theme> mTheme{ nullptr };

};

#define APP (qobject_cast<App*>(qApp))
