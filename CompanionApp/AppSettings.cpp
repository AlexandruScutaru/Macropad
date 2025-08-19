#include "AppSettings.h"

#include "controllers/settings/HotkeyActions.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

static constexpr auto WINDOWSTATE_WIDTH = "windowState/width";
static constexpr auto WINDOWSTATE_HEIGHT = "windowState/height";
static constexpr auto WINDOWSTATE_NAVBAR_EXPANDED = "windowState/navBarExpanded";

static constexpr auto DEVICE = "device";
static constexpr auto KEYPAD = "keypad";
static constexpr auto PROFILE = "profile";
static constexpr auto ACTION_ID = "actionId";

static constexpr auto SLIDERS_ARRAY = "sliders";
static constexpr auto MIN = "min";
static constexpr auto MAX = "max";

static constexpr auto NUM_POTENTIOMETERS = 4;


AppSettings::AppSettings(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppSettings::AppSettings";

    readWindowState();
    readPotentiometersInfo();
    readProfileData();
}

AppSettings::~AppSettings() {
    qDebug() << "AppSettings::~AppSettings";
}


std::unique_ptr<QSettings> AppSettings::getSettings() {
    return std::make_unique<QSettings>(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
}


QSize AppSettings::windowSize() {
    return mWindowSize;
}

void AppSettings::saveWindowSize(const QSize& size) {
    mWindowSize = size;

    auto settings = getSettings();
    settings->setValue(WINDOWSTATE_WIDTH, size.width());
    settings->setValue(WINDOWSTATE_HEIGHT, size.height());
}

bool AppSettings::navBarExpanded() {
    return mNavBarExpanded;
}

void AppSettings::saveNavBarExpanded(bool expanded) {
    mNavBarExpanded = expanded;

    auto settings = getSettings();
    settings->setValue(WINDOWSTATE_NAVBAR_EXPANDED, mNavBarExpanded);
}

void AppSettings::readWindowState() {
    auto settings = getSettings();

    auto w = settings->value(WINDOWSTATE_WIDTH, 900).toInt();
    auto h = settings->value(WINDOWSTATE_HEIGHT, 540).toInt();
    mWindowSize = QSize(w, h);

    mNavBarExpanded = settings->value(WINDOWSTATE_NAVBAR_EXPANDED, true).toBool();
}

Potentiometers AppSettings::potentiometersInfo() {
    return mPotentiometersInfo;
}

void AppSettings::savePotentiometersInfo(const Potentiometers& potentiometers) {
    mPotentiometersInfo = potentiometers;

    auto settings = getSettings();
    settings->beginGroup(DEVICE);
    settings->beginWriteArray(SLIDERS_ARRAY);
    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        settings->setArrayIndex(i);
        settings->setValue(MIN, mPotentiometersInfo[i].min);
        settings->setValue(MAX, mPotentiometersInfo[i].max);
    }
    settings->endArray();
    settings->endGroup();
}

void AppSettings::readPotentiometersInfo() {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    auto size = settings->beginReadArray(SLIDERS_ARRAY);

    mPotentiometersInfo.clear();
    mPotentiometersInfo.resize(size);
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        auto min = settings->value(MIN, 0).toInt();
        auto max = settings->value(MAX, 1023).toInt();
        mPotentiometersInfo[i].min = min;
        mPotentiometersInfo[i].max = max;
    }
    settings->endArray();
    settings->endGroup();

    if (mPotentiometersInfo.size() == 0) {
        mPotentiometersInfo.resize(NUM_POTENTIOMETERS);
    }
}

QString AppSettings::profileData() {
    return mProfileData;
}

void AppSettings::saveProfileData(const QString& profile) {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    settings->beginGroup(KEYPAD);

    settings->setValue(PROFILE, profile);

    settings->endGroup();
    settings->endGroup();
}

void AppSettings::readProfileData() {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    settings->beginGroup(KEYPAD);

    mProfileData = settings->value(PROFILE, "").toString();

    settings->endGroup();
    settings->endGroup();
}
