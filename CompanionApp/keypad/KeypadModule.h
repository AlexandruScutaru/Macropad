#pragma once

#include "controller/ActionConfigController.h"
#include "controller/AvailableActionsController.h"
#include "controller/KeypadController.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

class AppSettings;


class KeypadModule : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit KeypadModule(QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadModule();

    Q_INVOKABLE ActionConfigController* getActionConfigController();
    Q_INVOKABLE AvailableActionsController* getAvailableActionsController();
    Q_INVOKABLE KeypadController* getKeypadController();

private:
    QPointer<ActionConfigController> mActionConfigController{ nullptr };
    QPointer<AvailableActionsController> mAvailableActionsController{ nullptr };
    QPointer<KeypadController> mKeypadController{ nullptr };

};
