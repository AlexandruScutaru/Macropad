#include "Utils.h"

#include <nlohmann/json.hpp>


namespace utils {
    namespace json {
        QVariant ReadVariant(const nlohmann::json& json, const QString& fieldName, Keypad::OptionType type) {
            switch (type) {
                case Keypad::OptionType::String:
                case Keypad::OptionType::Path:
                    return QString::fromStdString(json.value<std::string>(fieldName.toStdString(), ""));
                default:
                    break;
            }

            return {};
        }

        void WriteVariant(nlohmann::json& json, const QString& fieldName, Keypad::OptionType type, const QVariant& variant) {
            switch (type) {
                case Keypad::OptionType::String:
                case Keypad::OptionType::Path:
                    json[fieldName.toStdString()] = variant.toString().toStdString();
                    break;
                default:
                    break;
            }
        }
    }
}
