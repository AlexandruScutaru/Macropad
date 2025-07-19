#include "TrayIcon.h"

#include <QAction>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDebug>
#include <QPointer>
#include <QMenu>


TrayIcon::TrayIcon(QObject *parent)
    : QObject(parent)
{
    qDebug() << "TrayIcon::TrayIcon";

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "system tray not available";
        return;
    }

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

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, [this, thisPtr = QPointer(this)](QSystemTrayIcon::ActivationReason reason) {
        if (!thisPtr) {
            return;
        }

        if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
            emit activated();
        }
    });
}

TrayIcon::~TrayIcon() {
    qDebug() << "TrayIcon::~TrayIcon";
}
