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
static constexpr auto LAYERS_ARRAY = "layers";
static constexpr auto LAYER_COLOR = "color";
static constexpr auto KEYS = "keys";
static constexpr auto ACTION_ID = "actionId";

static constexpr auto SLIDERS_ARRAY = "sliders";
static constexpr auto MIN = "min";
static constexpr auto MAX = "max";

static constexpr auto NUM_POTENTIOMETERS = 4;
static constexpr auto NUM_LAYERS_DEFAULT = 3;


AppSettings::AppSettings(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AppSettings::AppSettings";

    readWindowState();
    readPotentiometersInfo();
    readLayersInfo();
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

Layers AppSettings::layersInfo() {
    return mLayersInfo;
}

void AppSettings::saveKeyAssignment(int layer, int key, const QString& action) {
    addLayersAsNeeded(layer);
    mLayersInfo[layer].keys[key].id = action;

    auto settings = getSettings();
    settings->beginGroup(DEVICE);
      settings->beginGroup(KEYPAD);
        settings->beginWriteArray(LAYERS_ARRAY, mLayersInfo.size());
          settings->setArrayIndex(layer);
            settings->beginWriteArray(KEYS, mLayersInfo[layer].keys.size());
              settings->setArrayIndex(key);
              settings->setValue(ACTION_ID, action);
            settings->endArray();
          settings->endArray();
      settings->endGroup();
    settings->endGroup();
}

void AppSettings::saveLayersInfo(const Layers& layers) {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    settings->beginGroup(KEYPAD);
    settings->beginWriteArray(LAYERS_ARRAY);
    for (int layer = 0; layer < mLayersInfo.size(); layer++) {
        settings->setArrayIndex(layer);
        settings->setValue(LAYER_COLOR, mLayersInfo[layer].color);

        const auto& keys = mLayersInfo[layer].keys;
        settings->beginWriteArray(KEYS);
        for (int key = 0; key < keys.size(); key++) {
            settings->setArrayIndex(key);
            settings->setValue(ACTION_ID, keys[key].id);
        }
        settings->endArray();
    }
    settings->endArray();
    settings->endGroup();
    settings->endGroup();
}

void AppSettings::readLayersInfo() {
    auto settings = getSettings();

    settings->beginGroup(DEVICE);
    settings->beginGroup(KEYPAD);
    auto layerCount = settings->beginReadArray(LAYERS_ARRAY);

    mLayersInfo.clear();
    mLayersInfo.resize(layerCount);
    for (int layer = 0; layer < layerCount; layer++) {
        settings->setArrayIndex(layer);
        auto keys = Keys();
        const auto keyCount = settings->beginReadArray(KEYS);
        for (int key = 0; key < keyCount; key++) {
            settings->setArrayIndex(key);
            keys[key].id = settings->value(ACTION_ID, "").toString();
        }
        settings->endArray();
        mLayersInfo[layer].keys = keys;
    }
    settings->endArray();
    settings->endGroup();
    settings->endGroup();
}

void AppSettings::addLayersAsNeeded(int layer) {
    const int diff = (layer + 1) - static_cast<int>(mLayersInfo.size());
    if (diff <= 0) {
        return;
    }

    for (auto i = 0; i < diff; i++) {
        mLayersInfo.push_back({});
    }

    saveLayersInfo(mLayersInfo);
}
