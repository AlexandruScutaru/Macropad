#include "SettingsService.h"
#include "app/AppSettings.h"
#include "app/theming/Theme.h"
#include "app/theming/ThemeLoader.h"

#include <QDebug>
#include <cassert>


using namespace settings;

SettingsService::SettingsService(AppSettings* appSettings, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
{
    qDebug() << "SettingsService::SettingsService";

    assert(mAppSettings && "null AppSettings pointer");

    mCurrentTheme = mAppSettings->themeName();
}

SettingsService::~SettingsService() {
    qDebug() << "SettingsService::~SettingsService";
}

void SettingsService::setNavBarExpanded(bool expanded) {
    mAppSettings->saveNavBarExpanded(expanded);
}

bool SettingsService::getNavBarExpanded() const {
    return mAppSettings->navBarExpanded();
}

void SettingsService::setWindowSize(const QSize& size) {
    mAppSettings->saveWindowSize(size);
}

QSize SettingsService::getWindowSize() const {
    return mAppSettings->windowSize();
}

QString SettingsService::getCurrentTheme() const {
    return mCurrentTheme;
}

QStringList SettingsService::getAvailableTabs() const {
    return {
        "Appearance"
    };
}

std::vector<settings::ThemeEntry> SettingsService::getAvailableThemes() const {
    const auto availableThemes = theme::Loader::GetAvailableThemes();

    std::vector<settings::ThemeEntry> themes;
    themes.reserve(availableThemes.length());

    for (const auto& themeName: availableThemes) {
        themes.emplace_back(themeName, QVariant::fromValue(theme::Loader::Load(themeName)));
    }

    return themes;
}

void SettingsService::changeTheme(const QString& name) {
    mAppSettings->saveThemeName(name);
    mCurrentTheme = name;

    emit changeThemeRequested(name);
}
