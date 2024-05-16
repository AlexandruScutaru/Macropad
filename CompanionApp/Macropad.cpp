#include "Macropad.h"
#include "TrayIcon.h"
#include "AudioOutputSwitcher.h"
#include "DevHelperController.h"
#include "DeviceController.h"
#include "NativeEventFilter.h"
#include "PotentiometersReader.h"
#include "WinApiWrapper.h"
#include "DebugChecker.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSettings>

static constexpr auto DEV_HELPER_QML_CONTAINER_NAME = "devHelperViewContainer";
static constexpr auto DEVICE_QML_CONTAINER_NAME = "deviceViewContainer";


Macropad::Macropad(QQmlApplicationEngine& engine, QObject* parent)
    : QObject(parent)
    , mQmlEngine(engine)
{
    qDebug() << "Macropad::Macropad";
}

Macropad::~Macropad() {
    qDebug() << "Macropad::~Macropad";
}

void Macropad::onInitialized(bool isDebug) {
    const auto qmlWindow = getMainWindowObject();

    mAudioOutputSwitcher = new AudioOutputSwitcher(this);
    mPotentiometersReader = new PotentiometersReader(this);
    mDeviceController = new DeviceController(mPotentiometersReader, this);

    initTrayIcon();
    initHotkey();

    if (isDebug) {
        initDevHelperView(qmlWindow);
    }

    initDeviceView(qmlWindow);
}

void Macropad::saveDevHelperExpandState(bool isExpanded) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.setValue("windowState/devHelperState", isExpanded);
}

bool Macropad::devHelperExpandState() {
    if (!IS_DEBUG) {
        return false;
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    return settings.value("windowState/devHelperState", false).toBool();
}

QSize Macropad::windowSize() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    auto w = settings.value("windowState/width", 640).toInt();
    auto h = settings.value("windowState/height", 380).toInt();

    return QSize(w, h);
}

void Macropad::saveWindowSize(int w, int h) {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.setValue("windowState/width", w);
    settings.setValue("windowState/height", h);
}

void Macropad::onHotKeyTriggered(HotKeys hotKey) {
    if (hotKey == HotKeys::CYCLE_AUDIO_OUTPUTS) {
        mAudioOutputSwitcher->onSwitchOutputRequested();
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

void Macropad::initHotkey() {
    WinApiWrapper::RegisterGlobalShortcut(HotKeys::CYCLE_AUDIO_OUTPUTS);

    auto nativeEventFilter = new NativeEventFilter(this);
    QObject::connect(nativeEventFilter, &NativeEventFilter::hotKeyTriggered, this, &Macropad::onHotKeyTriggered);
    qApp->installNativeEventFilter(static_cast<QAbstractNativeEventFilter*>(nativeEventFilter));
}
