#pragma once

#include "../KeypadTypes.h"

#include <QObject>
#include <QPointer>
#include <nlohmann/json_fwd.hpp>

#include <string>
#include <vector>

class AppSettings;


class KeypadService : public QObject {
    Q_OBJECT
public:
    KeypadService(QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadService();

    void loadSavedProfile();
    void saveProfile(const Keypad::Profile& profile);

    const Keypad::AvailableActions& getAvailableActions() const;
    const Keypad::Profile& getCurrentProfile() const;

signals:
    void profileLoaded(const Keypad::Profile& profile);
    void actionAssigned(int layer, int key, const Keypad::Action& action);
    void actionConfigChanged(int layer, int key, const Keypad::Action& action);
    void actionTriggered(const Keypad::Action& action);

public slots:
    void onActionAssignRequested(int layer, int key, const QString& actionName);
    void onKeySelected(int layer, int key);
    void onKeyTriggered(int layer, int key);
    void onConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value);

private:
    void loadAvailableActions();

    QPointer<AppSettings> mAppSettings{ nullptr };

    Keypad::AvailableActions mAvailableActions;
    Keypad::Profile mCurrentProfile;

    static QVariant JsonReadVariant(const nlohmann::json& json, const std::string& fieldName, Keypad::OptionType type);
    static void JsonWriteVariant(nlohmann::json& json, const std::string& fieldname, Keypad::OptionType type, const QVariant& variant);

};
