#pragma once

#include <QJsonObject>
#include <QString>
#include <QStringList>

#include <optional>


namespace theme {
    class Theme;

    class Loader {
    public:
        using SetterFunc = void (Theme::*)(const QString&);

        static void Load(const QString& name, Theme& theme);
        static QStringList GetAvailableThemes();

    private:
        Loader() {};

        static std::optional<QJsonObject> LoadThemesJson(const QString& filePath);
        static void SetColor(const QJsonValue& json, const QString& name, Theme& theme, SetterFunc setter);

    };
}
