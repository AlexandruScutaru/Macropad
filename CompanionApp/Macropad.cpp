#include "Macropad.h"
#include "TrayIcon.h"
#include "AudioOutputSwitcher.h"
#include "DevHelperController.h"
#include "NativeEventFilter.h"
#include "WinApiWrapper.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto DEV_HELPER_QML_CONTAINER_NAME = "devHelperViewContainer";


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
    mAudioOutputSwitcher = new AudioOutputSwitcher(this);

    initTrayIcon();
    initHotkey();

    if (isDebug) {
        initDevHelperView();
    }
}


void Macropad::onHotKeyTriggered(HotKeys hotKey) {
    if (hotKey == HotKeys::CYCLE_AUDIO_OUTPUTS) {
        mAudioOutputSwitcher->onSwitchOutputRequested();
    }
}

void Macropad::initTrayIcon() {
    auto trayIcon = new TrayIcon(this);
    QObject::connect(trayIcon, &TrayIcon::activated, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::showActionTriggered, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::quitActionTriggered, qApp, &QApplication::quit);
}

void Macropad::initDevHelperView() {
    const auto qmlWindow = mQmlEngine.rootObjects().constFirst();
    if (!qmlWindow) {
        qWarning() << "cannot get main window object";
        return;
    }

    if (auto devHelperViewContainer = qmlWindow->findChild<QObject*>(DEV_HELPER_QML_CONTAINER_NAME); devHelperViewContainer) {
        auto devHelperController = new DevHelperController(this);
        QObject::connect(devHelperController, &DevHelperController::switchOutputRequested, mAudioOutputSwitcher, &AudioOutputSwitcher::onSwitchOutputRequested);

        QQmlComponent devView(&mQmlEngine, QStringLiteral("MacropadCompanion/DevHelperView.qml"));
        auto component = devView.createWithInitialProperties(QVariantMap{{ "controller", QVariant::fromValue<DevHelperController*>(devHelperController) }});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(devHelperViewContainer));
    }
}

void Macropad::initHotkey() {
    WinApiWrapper::RegisterGlobalShortcut(HotKeys::CYCLE_AUDIO_OUTPUTS);

    auto nativeEventFilter = new NativeEventFilter(this);
    QObject::connect(nativeEventFilter, &NativeEventFilter::hotKeyTriggered, this, &Macropad::onHotKeyTriggered);
    qApp->installNativeEventFilter(static_cast<QAbstractNativeEventFilter*>(nativeEventFilter));
}
