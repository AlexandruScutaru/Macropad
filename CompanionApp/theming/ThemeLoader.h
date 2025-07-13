#pragma once

#include <QJsonObject>
#include <QString>

#include <functional>


class Theme;

class ThemeLoader {
public:
    using SetterFunc = void (Theme::*)(const QString&);
    static Theme* LoadTheme(const QString& uri, const QString& name);

private:
    ThemeLoader() {};
    static void SetColor(const QJsonValue& json, const QString& name, Theme* theme, SetterFunc setter);

};
