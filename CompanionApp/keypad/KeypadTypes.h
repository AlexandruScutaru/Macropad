#pragma once

#include <QtQml>
#include <QString>
#include <QVariant>

#include <optional>
#include <unordered_map>


namespace Keypad {
    Q_NAMESPACE
    QML_ELEMENT
    enum OptionType {
        String = 0
    };
    Q_ENUM_NS(OptionType);

    struct Config {
        QString name;
        QString displayName;
        QString tooltip;
        OptionType type{ OptionType::String };
        QVariant value;
    };
    using Configs = std::vector<Config>;

    struct Action {
        QString name;
        QString displayName;
        QString tooltip;
        QString icon;
        Configs configs;
    };
    using ActionsMap = std::unordered_map<QString, Action>;

    struct SectionEntry {
        QString displayName;
        QString iconName;
        std::vector<QString> actions;
    };
    using Sections = std::vector<SectionEntry>;

    struct AvailableActions {
        ActionsMap actionsMap;
        Sections sections;

        std::optional<Keypad::Action> getAction(const QString& name) const {
            if (const auto& action = actionsMap.find(name); action != actionsMap.end()) {
                return action->second;
            }
        
            return std::nullopt;
        }
    };

    struct Layer {
        QString color;
        std::vector<Action> actions;
    };
    using Layers = std::vector<Layer>;

    struct Profile {
        QString name;
        Layers layers;
    };
}
