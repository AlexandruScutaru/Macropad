#include "Macropad.h"

#include "audio/AudioOutputSwitcher.h"
#include "Config.h"
#include "controllers/DevHelperController.h"
#include "controllers/DeviceController.h"
#include "controllers/Settings/HotkeyActions.h"
#include "hid/PotentiometersReader.h"
#include "misc/DebugChecker.h"
#include "os/windows/NativeEventFilter.h"
#include "os/windows/WinApiWrapper.h"
#include "tray/TrayIcon.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto DEV_HELPER_QML_CONTAINER_NAME = "devHelperViewContainer";
static constexpr auto DEVICE_QML_CONTAINER_NAME = "deviceViewContainer";


Macropad::Macropad(QQmlApplicationEngine& engine, Config* config, QObject* parent)
    : QObject(parent)
    , mQmlEngine(engine)
    , mConfig(config)
{
    qDebug() << "Macropad::Macropad";

    assert(mConfig && "Ivalid config");
}

Macropad::~Macropad() {
    qDebug() << "Macropad::~Macropad";
}

void Macropad::onInitialized(bool isDebug) {
    const auto qmlWindow = getMainWindowObject();

    mAudioOutputSwitcher = new AudioOutputSwitcher(this);
    mPotentiometersReader = new PotentiometersReader(this);
    mDeviceController = new DeviceController(mPotentiometersReader, mConfig, this);

    initTrayIcon();
    initHotkeysFilter();

    if (isDebug) {
        initDevHelperView(qmlWindow);
    }

    initDeviceView(qmlWindow);
}

void Macropad::saveDevHelperExpandState(bool isExpanded) {
    mConfig->saveDevHelperState(isExpanded);
}

bool Macropad::devHelperExpandState() {
    if (!IS_DEBUG) {
        return false;
    }

    return mConfig->devHelperState();
}

QSize Macropad::windowSize() {
    return mConfig->windowSize();
}

void Macropad::saveWindowSize(int w, int h) {
    mConfig->saveWindowSize({ w, h });
}

void Macropad::onHotkeyTriggered(int key) {
    const auto action = mConfig->keyToHotkeyAction(key);

    switch (action) {
        case Hotkeys::Actions::CYCLE_AUDIO_OUTPUTS:
            mAudioOutputSwitcher->onSwitchOutputRequested();
            return;
        default:
            return;
    }
}

QObject* const Macropad::getMainWindowObject() {
    const auto qmlWindow = mQmlEngine.rootObjects().constFirst();
    assert(qmlWindow && "Couldn't get main window object");
    return qmlWindow;
}

void Macropad::initTrayIcon() {
    auto trayIcon = new TrayIcon(this);
    QObject::connect(trayIcon, &TrayIcon::activated, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::showActionTriggered, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::quitActionTriggered, qApp, &QApplication::quit);
}

void Macropad::initDevHelperView(const QObject* const qmlWindow) {
    if (auto devHelperViewContainer = qmlWindow->findChild<QObject*>(DEV_HELPER_QML_CONTAINER_NAME); devHelperViewContainer) {
        auto devHelperController = new DevHelperController(this);
        QObject::connect(mDeviceController, &DeviceController::slidersChanged, devHelperController, &DevHelperController::onSlidersUpdated);
        QObject::connect(devHelperController, &DevHelperController::switchOutputRequested, mAudioOutputSwitcher, &AudioOutputSwitcher::onSwitchOutputRequested);

        QQmlComponent devView(&mQmlEngine, QStringLiteral(":/qt/qml/MacropadCompanion/DevHelperView.qml"));
        auto component = devView.createWithInitialProperties(QVariantMap{{ "controller", QVariant::fromValue<DevHelperController*>(devHelperController) }});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(devHelperViewContainer));
    }
}

void Macropad::initDeviceView(const QObject* const qmlWindow) {
    if (auto deviceViewContainer = qmlWindow->findChild<QObject*>(DEVICE_QML_CONTAINER_NAME); deviceViewContainer) {
        QQmlComponent devView(&mQmlEngine, QStringLiteral(":/qt/qml/MacropadCompanion/DeviceStackView.qml"));
        auto component = devView.createWithInitialProperties(QVariantMap{{ "controller", QVariant::fromValue<DeviceController*>(mDeviceController) }});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(deviceViewContainer));
    }
}

void Macropad::initHotkeysFilter() {
    auto nativeEventFilter = new NativeEventFilter(this);
    QObject::connect(nativeEventFilter, &NativeEventFilter::hotkeyTriggered, this, &Macropad::onHotkeyTriggered);
    qApp->installNativeEventFilter(static_cast<QAbstractNativeEventFilter*>(nativeEventFilter));
}
