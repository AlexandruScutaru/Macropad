#include "SettingsService.h"
#include "app/Config.h"
#include "app/theming/Theme.h"
#include "app/theming/ThemeLoader.h"

#include <QDebug>
#include <cassert>

static constexpr auto CONFIG_WIDTH_KEY           = "window-state/width";
static constexpr auto CONFIG_HEIGHT_KEY          = "window-state/height";
static constexpr auto CONFIG_NAVBAR_EXPANDED_KEY = "window-state/nav-bar-expanded";
static constexpr auto CONFIG_THEME_KEY           = "settings/theme";

using namespace settings;

SettingsService::SettingsService(Config* config, QObject* parent)
    : QObject(parent)
    , mConfig(config)
{
    qDebug() << "SettingsService::SettingsService";

    assert(mConfig && "null Config pointer");

    mCurrentTheme = mConfig->getProperty(CONFIG_THEME_KEY, QVariant(QStringLiteral("dark"))).toString();

    const auto availableThemeNames = theme::Loader::GetAvailableThemes();
    mAvailableThemes.reserve(availableThemeNames.size());

    for (const auto& themeName: availableThemeNames) {
        auto* previewTheme = new theme::Theme(this);
        theme::Loader::Load(themeName, *previewTheme);
        mAvailableThemes.emplace_back(themeName, QVariant::fromValue(previewTheme));
    }
}

SettingsService::~SettingsService() {
    qDebug() << "SettingsService::~SettingsService";
}

void SettingsService::setNavBarExpanded(bool expanded) {
    mConfig->setProperty(CONFIG_NAVBAR_EXPANDED_KEY, QVariant(expanded));
}

bool SettingsService::getNavBarExpanded() const {
    return mConfig->getProperty(CONFIG_NAVBAR_EXPANDED_KEY, QVariant(true)).toBool();
}

void SettingsService::setWindowSize(const QSize& size) {
    mConfig->setProperty(CONFIG_WIDTH_KEY, QVariant(size.width()));
    mConfig->setProperty(CONFIG_HEIGHT_KEY, QVariant(size.height()));
}

QSize SettingsService::getWindowSize() const {
    const auto width = mConfig->getProperty(CONFIG_WIDTH_KEY, QVariant(900)).toInt();
    const auto height = mConfig->getProperty(CONFIG_HEIGHT_KEY, QVariant(540)).toInt();

    return QSize(width, height);
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
    return mAvailableThemes;
}

void SettingsService::changeTheme(const QString& name) {
    mConfig->setProperty(CONFIG_THEME_KEY, QVariant(name));
    mCurrentTheme = name;

    emit changeThemeRequested(name);
}
