#include "Macropad.h"

#include "action-handlers/system/SystemActions.h"
#include "AppSettings.h"
#include "keypad/KeypadModule.h"
#include "misc/DebugChecker.h"
#include "theming/ThemeLoader.h"
#include "tray/TrayIcon.h"
#include "os/IPlatform.h"
#include "hid/Device.h"


#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto QML_APP_CONTAINER_NAME = "appStackViewContainer";
static constexpr auto THEMES_URI = ":/resources/themes.json";

static constexpr auto VID = 0xFEED;
static constexpr auto PID = 0xB00B;
static constexpr auto USAGE_PAGE = 0xFF60;
static constexpr auto USAGE_ID = 0x61;


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

    if (mTheme) {
        delete mTheme;
        mTheme = nullptr;
    }
}

void Macropad::init(const MacropadConfig& config) {
    mConfig = config;
    initTrayIcon();

    if (mConfig.isPlayground) {
        return;
    }

    mHidDevice = new hid::Device(this);
    QObject::connect(mHidDevice, &hid::Device::deviceConnected, this, &Macropad::deviceConnected);
    QObject::connect(mHidDevice, &hid::Device::deviceNotFound, this, &Macropad::deviceNotFound);

    mKeypadModule = new KeypadModule(mAppSettings, this);

    // TODO: get theme from saved settings
    loadTheme(ThemeVariant::Dark);
    initAppStackView(getMainWindowObject());
    initActionHandlers();
}

Theme* Macropad::getTheme() {
    // load default which is Dark at this stage
    if (!mTheme) {
        loadTheme(ThemeVariant::Dark);
    }

    return mTheme.data();
}

void Macropad::connectToDevice() {
    if (mConfig.isSkipPhysicalDevice) {
        emit deviceConnected();
        return;
    }

    if (mHidDevice) {
        mHidDevice->connect(VID, PID, USAGE_PAGE, USAGE_ID);
    }
}

KeypadModule* Macropad::getKeypadModule() {
    return mKeypadModule;
}

QSize Macropad::windowSize() {
    return mAppSettings->windowSize();
}

void Macropad::saveWindowSize(int w, int h) {
    mAppSettings->saveWindowSize({ w, h });
}

bool Macropad::navBarExpanded() {
    return mAppSettings->navBarExpanded();
}

void Macropad::saveNavBarExpanded(bool expanded) {
    mAppSettings->saveNavBarExpanded(expanded);
}

QObject* const Macropad::getMainWindowObject() {
    const auto qmlWindow = mQmlEngine.rootObjects().constFirst();
    assert(qmlWindow && "Couldn't get main window object");
    return qmlWindow;
}

void Macropad::loadTheme(ThemeVariant variant) {
    if (mTheme) {
        mTheme->deleteLater();
    }

    mTheme = QPointer(ThemeLoader::LoadTheme(THEMES_URI, variant));
    emit themeChanged(mTheme.data());
}

void Macropad::initActionHandlers() {
    auto platform = osal::CreatePlatform();

    mActionHandlers.push_back(std::make_shared<SystemActions>(platform));

    for (const auto& handler: mActionHandlers) {
        mKeypadModule->registerHandler(handler);
    }
}

void Macropad::initTrayIcon() {
    auto trayIcon = new TrayIcon(this);
    QObject::connect(trayIcon, &TrayIcon::activated, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::showActionTriggered, this, &Macropad::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::quitActionTriggered, qApp, &QApplication::quit);
}

void Macropad::initAppStackView(const QObject* const qmlWindow) {
    if (auto deviceViewContainer = qmlWindow->findChild<QObject*>(QML_APP_CONTAINER_NAME); deviceViewContainer) {
        QQmlComponent deviceView(&mQmlEngine, QStringLiteral(":/qt/qml/MacropadCompanion/AppStackView.qml"));
        if (deviceView.isError() || deviceView.isNull()) {
            qDebug() << "Cannot create AppStackView.qml: " << deviceView.errors();
            return;
        }

        auto component = deviceView.createWithInitialProperties(/*QVariantMap{{ "mainController", QVariant::fromValue<MainController*>(mMainController) }}*/ {});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(deviceViewContainer));
    }
}
