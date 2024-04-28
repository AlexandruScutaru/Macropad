#pragma once

#include <QObject>


class TrayIcon : public QObject {
    Q_OBJECT
public:
    explicit TrayIcon(QObject* parent = nullptr);
    ~TrayIcon();

signals:
    void activated();
    void showActionTriggered();
    void quitActionTriggered();

};
