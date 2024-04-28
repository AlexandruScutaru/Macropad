#pragma once

#include <QObject>
#include <QQmlEngine>


class DevHelperController : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DevHelperController(QObject *parent = nullptr);
    ~DevHelperController();

    Q_INVOKABLE void onSwitchOutputClicked();

};
