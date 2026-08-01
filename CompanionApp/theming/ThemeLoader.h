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
        static Theme* Load(const QString& uri, Type type);

    private:
        Loader() {};

        static void SetColor(const QJsonValue& json, const QString& name, Theme* theme, SetterFunc setter);
        static QString ThemeNameFromType(Type type);

    };
}
