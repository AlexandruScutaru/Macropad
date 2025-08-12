#pragma once

#include "KeypadTypes.h"

#include <QObject>
#include <QPointer>


class KeypadController;
class AppSettings;

class KeypadModule : public QObject {
    Q_OBJECT
public:
    explicit KeypadModule(QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadModule();

    KeypadController* getController();

private:
    static QString ComputeActionId(const QString& section, const QString& action);

    Keypad::Sections loadAvailableActions();

    QPointer<KeypadController> mController{ nullptr };

};
