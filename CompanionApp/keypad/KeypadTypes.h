#pragma once

#include <QtQml>
#include <QString>
#include <QVariant>

#include <tuple>
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
        QString tooltip;
        OptionType type{ OptionType::String };
        QVariant value;
    };
    using Configs = std::vector<Config>;

    struct Action {
        QString id;
        QString name;
        QString tooltip;
        QString icon;
        Configs config;
    };
    using ActionsMap = std::unordered_map<QString, Action>;

    struct SectionEntry {
        QString name;
        QString iconName;
        std::vector<QString> actions;
    };
    using Sections = std::vector<SectionEntry>;

    using AvailableActions = std::tuple<ActionsMap, Sections>;

    struct Layer {
        QString color;
        std::vector<Action> actions;
    };
    using Layers = std::vector<Layer>;
}
