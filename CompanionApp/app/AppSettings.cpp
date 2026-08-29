#include "AppSettings.h"
#include "app/App.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>


static constexpr auto WINDOWSTATE_GROUP = "window-state";
static constexpr auto WIDTH = "width";
static constexpr auto HEIGHT = "height";
static constexpr auto NAVBAR_EXPANDED = "nav-bar-expanded";

static constexpr auto SETTINGS_GROUP = "settings";
static constexpr auto THEME = "theme";

static constexpr auto DEVICE_GROUP = "device";
static constexpr auto KEYPAD_GROUP = "keypad";
static constexpr auto PROFILE = "profile";


AppSettings::AppSettings(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppSettings::AppSettings";

    readWindowState();
    readTheme();
    readProfileData();
}

AppSettings::~AppSettings() {
    qDebug() << "AppSettings::~AppSettings";
}


std::unique_ptr<QSettings> AppSettings::getSettings() {
    auto orgName = APP->organizationName();
    auto appName = APP->applicationName();

    return std::make_unique<QSettings>(QSettings::IniFormat, QSettings::UserScope, orgName, appName);
}


QSize AppSettings::windowSize() {
    return mWindowSize;
}

void AppSettings::saveWindowSize(const QSize& size) {
    mWindowSize = size;

    auto settings = getSettings();
    settings->beginGroup(WINDOWSTATE_GROUP);

    settings->setValue(WIDTH, size.width());
    settings->setValue(HEIGHT, size.height());

    settings->endGroup();
}

QString AppSettings::themeName() {
    return mThemeName;
}

void AppSettings::saveThemeName(const QString& theme) {
    mThemeName = theme;

    auto settings = getSettings();
    settings->beginGroup(SETTINGS_GROUP);

    settings->setValue(THEME, theme);

    settings->endGroup();
}

bool AppSettings::navBarExpanded() {
    return mNavBarExpanded;
}

void AppSettings::saveNavBarExpanded(bool expanded) {
    mNavBarExpanded = expanded;

    auto settings = getSettings();
    settings->beginGroup(WINDOWSTATE_GROUP);

    settings->setValue(NAVBAR_EXPANDED, mNavBarExpanded);

    settings->endGroup();
}

void AppSettings::readWindowState() {
    auto settings = getSettings();

    settings->beginGroup(WINDOWSTATE_GROUP);

    auto w = settings->value(WIDTH, 900).toInt();
    auto h = settings->value(HEIGHT, 540).toInt();
    mWindowSize = QSize(w, h);
    mNavBarExpanded = settings->value(NAVBAR_EXPANDED, true).toBool();

    settings->endGroup();
}

void AppSettings::readTheme() {
    auto settings = getSettings();
    settings->beginGroup(SETTINGS_GROUP);

    mThemeName = settings->value(THEME, "dark").toString();

    settings->endGroup();
}

QString AppSettings::profileData() {
    return mProfileData;
}

void AppSettings::saveProfileData(const QString& profile) {
    auto settings = getSettings();
    settings->beginGroup(DEVICE_GROUP);
    settings->beginGroup(KEYPAD_GROUP);

    settings->setValue(PROFILE, profile);

    settings->endGroup();
    settings->endGroup();
}

void AppSettings::readProfileData() {
    auto settings = getSettings();
    settings->beginGroup(DEVICE_GROUP);
    settings->beginGroup(KEYPAD_GROUP);

    mProfileData = settings->value(PROFILE, "").toString();

    settings->endGroup();
    settings->endGroup();
}
