#pragma once

#include "../KeypadTypes.h"

#include <QObject>
#include <QPointer>

#include <string>
#include <vector>

class AppSettings;


class KeypadService : public QObject {
    Q_OBJECT
public:
    KeypadService(QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadService();

    void loadSavedProfile(const Keypad::AvailableActions& availableActions);
    void saveProfile(const Keypad::Profile& profile);

signals:
    void profileLoaded(const Keypad::Profile& profile);


private:
    QPointer<AppSettings> mAppSettings{ nullptr };

};
