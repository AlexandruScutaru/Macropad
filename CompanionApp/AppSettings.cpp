#include "AppSettings.h"

#include "controllers/settings/HotkeyActions.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

static constexpr auto WINDOWSTATE_WIDTH = "windowState/width";
static constexpr auto WINDOWSTATE_HEIGHT = "windowState/height";
static constexpr auto WINDOWSTATE_DEVHELPER_STATE = "windowState/devHelperState";

static constexpr auto DEVICE_LAST_DEVICE_PATH = "device/lastDevicePath";

static constexpr auto HOTKEYS = "hotkeys";
static constexpr auto ACTIONS = "actions";
static constexpr auto ACTION = "action";
static constexpr auto KEY = "key";

static constexpr auto DEVICE = "device";
static constexpr auto SLIDERS = "sliders";
static constexpr auto MIN = "min";
static constexpr auto MAX = "max";


AppSettings::AppSettings(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppSettings::AppSettings";

    readWindowState();
    readDeviceConnectionData();
    readHotkeyActions();
    readPotentiometersInfo();
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

bool AppSettings::devHelperState() {
    return mDevHelperState;
}

void AppSettings::saveDevHelperState(bool state) {
    mDevHelperState = state;

    auto settings = getSettings();
    settings->setValue(WINDOWSTATE_DEVHELPER_STATE, state);
}

QString AppSettings::lastDevicePath() {
    return mLastDevicePath;
}

void AppSettings::saveLastDevicePath(const QString& path) {
    mLastDevicePath = path;

    auto settings = getSettings();
    settings->setValue(DEVICE_LAST_DEVICE_PATH, path);
}

void AppSettings::clearLastDevicePath() {
    mLastDevicePath = "";
    auto settings = getSettings();
    settings->remove(DEVICE_LAST_DEVICE_PATH);
}

HotkeyActionMap AppSettings::hotkeyActionMap() {
    return mHotkeyActionMap;
}

void AppSettings::saveHotkeyActionMap(const HotkeyActionMap& hotkeyActionMap) {
    mHotkeyActionMap = hotkeyActionMap;

    auto settings = getSettings();
    settings->beginGroup(HOTKEYS);
    settings->beginWriteArray(ACTIONS);

    // unfortunate naming, the key here is actually a keyboard button :shrug:
    auto index = 0;
    for (const auto& [action, key]: mHotkeyActionMap) {
        settings->setArrayIndex(index++);
        const auto actionVal = static_cast<int>(action);
        settings->setValue(ACTION, actionVal);
        settings->setValue(KEY, key);
    }

    settings->endArray();
    settings->endGroup();
}

Hotkeys::Actions AppSettings::keyToHotkeyAction(int key) {
    const auto it = std::find_if(mHotkeyActionMap.begin(), mHotkeyActionMap.end(), [key](const auto& entry) {
        return entry.second == key;
    });

    if (it != mHotkeyActionMap.end()) {
        return it->first;
    }

    return Hotkeys::Actions::UNKNOWN;
}

Potentiometers AppSettings::potentiometersInfo() {
    return mPotentiometersInfo;
}

void AppSettings::savePotentiometersInfo(const Potentiometers& potentiometers) {
    mPotentiometersInfo = potentiometers;

    auto settings = getSettings();
    settings->beginGroup(DEVICE);
    settings->beginWriteArray(SLIDERS);
    for (int i = 0; i < mPotentiometersInfo.size(); i++) {
        settings->setArrayIndex(i);
        settings->setValue(MIN, mPotentiometersInfo[i].min);
        settings->setValue(MAX, mPotentiometersInfo[i].max);
    }
    settings->endArray();
    settings->endGroup();
}

void AppSettings::readWindowState() {
    auto settings = getSettings();

    auto w = settings->value(WINDOWSTATE_WIDTH, 640).toInt();
    auto h = settings->value(WINDOWSTATE_HEIGHT, 380).toInt();
    mWindowSize = QSize(w, h);

    mDevHelperState = settings->value(WINDOWSTATE_DEVHELPER_STATE, false).toBool();
}

void AppSettings::readDeviceConnectionData() {
    auto settings = getSettings();
    mLastDevicePath = settings->value(DEVICE_LAST_DEVICE_PATH, "").toString();
}

void AppSettings::readHotkeyActions() {
    auto settings = getSettings();

    settings->beginGroup(HOTKEYS);
    auto savedActionsSize = settings->beginReadArray(ACTIONS);

    for (int i = 0; i < savedActionsSize; i++) {
        settings->setArrayIndex(i);
        auto action = settings->value(ACTION);
        auto key = settings->value(KEY);

        if (!action.isValid() || !key.isValid()) {
            settings->remove("");
            settings->endGroup();
            return;
        }

        mHotkeyActionMap[static_cast<Hotkeys::Actions>(action.toInt())] = key.toInt();
    }

    settings->endArray();
    settings->endGroup();
}

void AppSettings::readPotentiometersInfo() {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    auto size = settings->beginReadArray(SLIDERS);

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
}
