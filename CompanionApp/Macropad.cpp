#include "Macropad.h"

#include "audio/AudioOutputSwitcher.h"
#include "AppSettings.h"
#include "controllers/MainController.h"
#include "controllers/settings/HotkeyActions.h"
#include "hid/PotentiometersReader.h"
#include "misc/DebugChecker.h"
#include "tray/TrayIcon.h"
#include "theming/ThemeLoader.h"

#include <QApplication>
#include <QDebug>
#include <QQmlComponent>
#include <QQuickItem>

static constexpr auto QML_APP_CONTAINER_NAME = "appStackViewContainer";
static constexpr auto THEMES_URI = ":/resources/themes.json";


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

void Macropad::onInitialized(const MacropadConfig& config) {
    mConfig = config;
    initTrayIcon();

    if (mConfig.isPlayground) {
        return;
    }

    const auto qmlWindow = getMainWindowObject();
    mAudioOutputSwitcher = new AudioOutputSwitcher(this);
    mPotentiometersReader = new PotentiometersReader(this);
    mMainController = new MainController(mPotentiometersReader, mAppSettings->potentiometersInfo(), mConfig.isSkipPhysicalDevice, this);

    QObject::connect(mMainController, &MainController::switchOutputRequested, mAudioOutputSwitcher, &AudioOutputSwitcher::onSwitchOutputRequested);

    // TODO: get theme from saved settings
    loadTheme(ThemeVariant::Dark);
    initAppStackView(qmlWindow);
}

Theme* Macropad::getTheme() {
    // load default which is Dark at this stage
    if (!mTheme) {
        loadTheme(ThemeVariant::Dark);
    }

    return mTheme.data();
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
            qDebug() << "Cannot create AppStackView.qml";
            return;
        }

        auto component = deviceView.createWithInitialProperties(QVariantMap{{ "mainController", QVariant::fromValue<MainController*>(mMainController) }});
        auto item = qobject_cast<QQuickItem*>(component);
        item->setParentItem(qobject_cast<QQuickItem*>(deviceViewContainer));
    }
}
