#pragma once

#include <QObject>
#include <QSystemTrayIcon>


class TrayIcon : public QObject {
    Q_OBJECT
public:
    explicit TrayIcon(QObject* parent = nullptr);
    ~TrayIcon();

signals:
    void activated(QSystemTrayIcon::ActivationReason reason);
    void showActionTriggered();
    void quitActionTriggered();

};
