#include "Macropad.h"

#include "audio/AudioOutputSwitcher.h"
#include "AppSettings.h"
#include "controllers/MainController.h"
#include "controllers/settings/HotkeyActions.h"
#include "hid/PotentiometersReader.h"
#include "misc/DebugChecker.h"
#include "tray/TrayIcon.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto DEVICE_QML_CONTAINER_NAME = "deviceStackViewContainer";


Macropad::Macropad(QQmlApplicationEngine& engine, AppSettings* appSettings, QObject* parent)
    : QObject(parent)
    , mQmlEngine(engine)
    , mAppSettings(appSettings)
{
    qDebug() << "Macropad::Macropad";

    assert(mAppSettings && "Invalid appSettings");
}

Macropad::~Macropad() {
    qDebug() << "Macropad::~Macropad";
}

void Macropad::onInitialized(const MacropadConfig& config) {
    mConfig = config;

    const auto qmlWindow = getMainWindowObject();
    mAudioOutputSwitcher = new AudioOutputSwitcher(this);
    mPotentiometersReader = new PotentiometersReader(this);
    mMainController = new MainController(mPotentiometersReader, mAppSettings, mConfig.isSkipPhysicalDevice, this);

    QObject::connect(mMainController, &MainController::switchOutputRequested, mAudioOutputSwitcher, &AudioOutputSwitcher::onSwitchOutputRequested);

    initTrayIcon();
    initDeviceView(qmlWindow);
}

QSize Macropad::windowSize() {
    return mAppSettings->windowSize();
}

void Macropad::saveWindowSize(int w, int h) {
    mAppSettings->saveWindowSize({ w, h });
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

void Macropad::initDeviceView(const QObject* const qmlWindow) {
    if (auto deviceViewContainer = qmlWindow->findChild<QObject*>(DEVICE_QML_CONTAINER_NAME); deviceViewContainer) {
        QQmlComponent deviceView(&mQmlEngine, QStringLiteral(":/qt/qml/MacropadCompanion/DeviceStackView.qml"));
        if (deviceView.isError() || deviceView.isNull()) {
            qDebug() << "Cannot create DeviceStackView.qml";
            return;
        }

        auto component = deviceView.createWithInitialProperties(QVariantMap{{ "mainController", QVariant::fromValue<MainController*>(mMainController) }});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(deviceViewContainer));
    }
}
