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

    static QString Id(const QString& section, const QString& action);

private:
    Keypad::AvailableActions loadAvailableActions();

    QPointer<KeypadController> mController{ nullptr };

};
