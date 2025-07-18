#pragma once

#include <QJsonObject>
#include <QString>

#include <functional>


class Theme;
enum class ThemeVariant;

class ThemeLoader {
public:
    using SetterFunc = void (Theme::*)(const QString&);
    static Theme* LoadTheme(const QString& uri, ThemeVariant variant);

private:
    ThemeLoader() {};

    static void SetColor(const QJsonValue& json, const QString& name, Theme* theme, SetterFunc setter);
    static QString ThemeNameFromVariant(ThemeVariant variant);

};
