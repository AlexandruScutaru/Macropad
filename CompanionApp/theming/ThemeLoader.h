#pragma once

#include <QJsonObject>
#include <QString>

#include <functional>

namespace theme {
    enum class Type;
    class Theme;

    class Loader {
    public:
        using SetterFunc = void (Theme::*)(const QString&);
        static Theme* Load(Type type);

        static QString ThemeNameFromType(Type type);
        static Type ThemeTypeFromName(const QString& name);

    private:
        Loader() {};

        static void SetColor(const QJsonValue& json, const QString& name, Theme* theme, SetterFunc setter);

    };
}
