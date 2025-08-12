#pragma once

#include <QString>
#include <QVariant>

#include <unordered_map>

namespace Keypad {
    struct ActionEntry {
        QString id;
        QString name;
        QString toolTip;
        QString iconName;
    };
    using Actions = std::vector<ActionEntry>;

    struct SectionEntry {
        QString name;
        QString iconName;
        Actions actions;
    };
    using Sections = std::vector<SectionEntry>;

    enum class FieldType {
        String = 0,
        Number
    };

    struct ConfigField {
        FieldType type;
        QString name;
        QString tooltip;
        QVariant value;
    };
    using ConfigFields = std::vector<ConfigField>;

    struct KeyEntry {
        QString actionId;
        QString actionName;
        QString actionIcon;
        ConfigFields config;
    };
    using Keys = std::vector<KeyEntry>;

    struct LayerEntry {
        QString color;
        Keys keys;
    };
    using Layers = std::vector<LayerEntry>;
}
