#include "TrayIcon.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMenu>


TrayIcon::TrayIcon(QObject *parent)
    : QObject(parent)
{
    qDebug() << "TrayIcon::TrayIcon";

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "system tray not available";
        return;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    QAction* showAction = new QAction("Show", this);
    QAction* quitAction = new QAction("Quit", this);

    QObject::connect(showAction, &QAction::triggered, this, &TrayIcon::showActionTriggered);
    QObject::connect(quitAction, &QAction::triggered, this, &TrayIcon::quitActionTriggered);

    QMenu* trayIconMenu = new QMenu("tray icon menu options");
    trayIconMenu->addAction(showAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/resources/app_icon.png"));
    trayIcon->setToolTip("Macropad Companion");
    trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::activated);
}

TrayIcon::~TrayIcon() {
    qDebug() << "TrayIcon::~TrayIcon";
}
