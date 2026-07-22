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

    void loadSavedProfile(const Keypad::AvailableActions& availableActions);
    void saveProfile(const Keypad::Profile& profile);

signals:
    void profileLoaded(const Keypad::Profile& profile);


private:
    QPointer<AppSettings> mAppSettings{ nullptr };

    static QVariant JsonReadVariant(const nlohmann::json& json, const std::string& fieldName, Keypad::OptionType type);
    static void JsonWriteVariant(nlohmann::json& json, const std::string& fieldname, Keypad::OptionType type, const QVariant& variant);

};
