#pragma once

#include "KeypadTypes.h"

#include <QVariant>
#include <nlohmann/json_fwd.hpp>

#include <string>


namespace utils{
    namespace json {
        QVariant ReadVariant(const nlohmann::json& json, const QString& fieldName, Keypad::OptionType type);
        void WriteVariant(nlohmann::json& json, const QString& fieldName, Keypad::OptionType type, const QVariant& variant);
    }
}
