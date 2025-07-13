#include "ThemeLoader.h"
#include "Theme.h"

#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


Theme* ThemeLoader::LoadTheme(const QString& uri, const QString& name) {
    const auto theme = new Theme();

    QFile file(uri);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file" << uri;
        return theme;
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (jsonDoc.isNull()) {
        qDebug() << "Cannot parse JSON file:" << parseError.errorString();
        return theme;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON document is not an object";
        return theme;
    }

    QJsonObject json = jsonDoc.object();
    if (json.isEmpty()) {
        qDebug() << "JSON is empty";
        return theme;
    }

    const auto themesData = json["themes"];
    if (!themesData.isObject()) {
        qDebug() << "'themes' json property is not an object";
        return theme;
    }

    const auto themeData = themesData[name];
    if (!themeData.isObject()) {
        qDebug() << "theme '" << name << "' is not an object";
        return theme;
    }

    if (const auto& text = themeData["text"]; text.isObject()) {
        SetColor(text, "primary", theme, &Theme::set_textPrimary);
        SetColor(text, "secondary", theme, &Theme::set_textSecondary);
        SetColor(text, "disabled", theme, &Theme::set_textDisabled);
    }

    if (const auto& buttons = themeData["buttons"]; buttons.isObject()) {
        if (const auto& primary = buttons["primary"]; primary.isObject()) {
            SetColor(primary, "normal", theme, &Theme::set_buttonPrimaryNormal);
            SetColor(primary, "hovered", theme, &Theme::set_buttonPrimaryHovered);
            SetColor(primary, "pressed", theme, &Theme::set_buttonPrimaryPressed);
            SetColor(primary, "disabled", theme, &Theme::set_buttonPrimaryDisabled);
        }

        if (const auto& secondary = buttons["secondary"]; secondary.isObject()) {
            SetColor(secondary, "normal", theme, &Theme::set_buttonSecondaryNormal);
            SetColor(secondary, "hovered", theme, &Theme::set_buttonSecondaryHovered);
            SetColor(secondary, "pressed", theme, &Theme::set_buttonSecondaryPressed);
            SetColor(secondary, "disabled", theme, &Theme::set_buttonSecondaryDisabled);
        }
    }

    if (const auto& background = themeData["background"]; background.isObject()) {
        SetColor(background, "primary", theme, &Theme::set_backgroundPrimary);
        SetColor(background, "secondary", theme, &Theme::set_backgroundSecondary);
        SetColor(background, "tertiary", theme, &Theme::set_backgroundTertiary);
    }

    if (const auto& other = themeData["other"]; other.isObject()) {
        if (const auto& primary = other["accentPrimary"]; primary.isObject()) {
            SetColor(primary, "normal", theme, &Theme::set_accentPrimaryNormal);
            SetColor(primary, "hovered", theme, &Theme::set_accentPrimaryHovered);
        }

        SetColor(other, "error", theme, &Theme::set_error);
    }

    return theme;
}

void ThemeLoader::SetColor(const QJsonValue& json, const QString& name, Theme* theme, ThemeLoader::SetterFunc setter) {
    if (const auto& color = json[name]; color.isString()) {
        ((*theme).*setter)(color.toString());
    }
}
