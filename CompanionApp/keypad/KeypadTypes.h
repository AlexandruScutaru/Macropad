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
        Unknown = 0,
        String
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
        QString id;
        QString sectionId;
        QString displayName;
        QString tooltip;
        QString iconName;
        Configs configs;
    };
    using ActionsMap = std::unordered_map<QString, Action>;
    using Actions = std::vector<Action>;

    struct Section {
        QString id;
        QString displayName;
        QString iconName;
        std::vector<QString> actions;
    };
    using Sections = std::vector<Section>;

    struct AvailableActions {
        ActionsMap actionsMap;
        Sections sections;

        std::optional<Keypad::Action> getAction(const QString& id) const {
            if (const auto& action = actionsMap.find(id); action != actionsMap.end()) {
                return action->second;
            }
        
            return std::nullopt;
        }
    };

    struct Layer {
        QString color;
        Actions actions;
    };
    using Layers = std::vector<Layer>;

    struct Profile {
        QString id;
        QString name;
        Layers layers;
    };
}
