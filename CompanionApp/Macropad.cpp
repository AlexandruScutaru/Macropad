#include "Macropad.h"

#include "action-handlers/system/SystemActions.h"
#include "AppSettings.h"
#include "keypad/KeypadModule.h"
#include "misc/DebugChecker.h"
#include "theming/ThemeLoader.h"
#include "tray/TrayIcon.h"
#include "os/IPlatform.h"
#include "hid/Device.h"
#include "settings/controller/SettingsController.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto QML_APP_CONTAINER_NAME = "appStackContainer";

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

    mSettingsController = new settings::SettingsController(this);
    QObject::connect(mSettingsController, &settings::SettingsController::changeThemeRequested, this, &Macropad::onThemeChangeRequested);

    mHidDevice = new hid::Device(this);
    QObject::connect(mHidDevice, &hid::Device::deviceConnected, this, &Macropad::deviceConnected);
    QObject::connect(mHidDevice, &hid::Device::deviceNotFound, this, &Macropad::deviceNotFound);

    mKeypadModule = new KeypadModule(mAppSettings, this);

    loadTheme(mAppSettings->themeName());
    initAppStack(getMainWindowObject());
    initActionHandlers();
}

theme::Theme* Macropad::getTheme() {
    if (!mTheme) {
        loadTheme(theme::DEFAULT_THEME_NAME);
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

void Macropad::loadTheme(const QString& name) {
    if (mTheme) {
        mTheme->deleteLater();
    }

    mTheme = QPointer(theme::Loader::Load(name));

    if (mSettingsController) {
        mSettingsController->setCurrentTheme(mTheme->getName());
    }

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

void Macropad::initAppStack(const QObject* const qmlWindow) {
    if (auto appStackContainer = qmlWindow->findChild<QObject*>(QML_APP_CONTAINER_NAME); appStackContainer) {
        QQmlComponent component(&mQmlEngine, QStringLiteral(":/qt/qml/MacropadCompanion/AppStack.qml"));
        if (component.isError() || component.isNull()) {
            qDebug() << "Cannot load AppStack.qml: " << component.errors();
            return;
        }

        auto object = component.createWithInitialProperties(QVariantMap{{ "settingsController", QVariant::fromValue<settings::SettingsController*>(mSettingsController) }});
        if (!object) {
            qDebug() << "Cannot create AppStack.qml instance:";
            for (const QQmlError &error : component.errors()) {
                qDebug().noquote() << error.toString() << "\n";
            }

            return;
        }

        auto item = qobject_cast<QQuickItem*>(object);
        if (!item) {
            qDebug() << "Cannot cast QObject* to QQuickItem*";
            return;
        }

        item->setParentItem(qobject_cast<QQuickItem*>(appStackContainer));
    }
}


void Macropad::onThemeChangeRequested(const QString& name) {
    loadTheme(name);
    mAppSettings->saveThemeName(name);
}
