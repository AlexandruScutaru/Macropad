#include "App.h"
#include "Theme.h"
#include "CmdArgs.h"
#include "theming/ThemeLoader.h"
#include "tray/TrayIcon.h"
#include "app/AppSettings.h"
#include "app/context/AppContext.h"

#include <QDebug>
#include <QIcon>
#include <QQmlContext>
#include <QQuickStyle>

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(ComponentsPlugin)


static constexpr auto CMD_ARG_SKIP_PHYSICAL_DEVICE = "--skipPhysicalDevice";
static constexpr auto CMD_ARG_PLAYGROUND = "--playground";


App::App(int &argc, char **argv)
    : QApplication(argc, argv)
{
    qDebug() << "App::App";
    
    setWindowIcon(QIcon(":/resources/app_icon.png"));
    setQuitOnLastWindowClosed(false);
    setOrganizationName("Macropad");
    setApplicationName("Companion");

    auto config = getConfig(argc, argv);
    mAppContext = new AppContext(new AppSettings(this), config, this);

    initQmlEngine(config);
    initTrayIcon();
}

App::~App() {
    qDebug() << "App::~App";

    if (mTheme) {
        delete mTheme;
        mTheme = nullptr;
    }
}


void App::onChangeThemeRequested(const QString& name) {
    if (mTheme && mTheme->getName() == name) {
        return;
    }

    if (mTheme) {
        mTheme->deleteLater();
    }

    mTheme = QPointer(theme::Loader::Load(name));

    mQmlEngine.rootContext()->setContextProperty("Theme", mTheme.data());
}

AppConfig App::getConfig(int& argc, char** argv) {
    bool isDebug = false;

#ifndef NDEBUG
    isDebug = true;
#endif

    CmdArgs cmdArgs(argc, argv, {
        CMD_ARG_SKIP_PHYSICAL_DEVICE,
        CMD_ARG_PLAYGROUND
    });

    const auto isPlayground = isDebug && cmdArgs.getFlag(CMD_ARG_PLAYGROUND);

    return {
        .isDebug = isDebug,
        .isSkipPhysicalDevice = isPlayground || cmdArgs.getFlag(CMD_ARG_SKIP_PHYSICAL_DEVICE),
        .isPlayground = isPlayground,
    };
}


void App::initQmlEngine(const AppConfig& config) {
    qmlRegisterSingletonInstance(
        "Macropad.AppContext",
        1, 0,
        "AppContext",
        mAppContext
    );

    QQuickStyle::setStyle("basic");

    QObject::connect(
        &mQmlEngine, &QQmlApplicationEngine::objectCreationFailed,
        this, [](const QUrl &url) {
            qDebug() << "QQmlApplicationEngine::objectCreationFailed for " << url;
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    QObject::connect(
        &mQmlEngine, &QQmlApplicationEngine::objectCreated,
        this, [this](QObject* obj, const QUrl& url) {
            if (!obj) {
                qDebug() << "QQmlApplicationEngine created object is null (for some reason) for " << url;
                QCoreApplication::exit(-2);
            }
        },
        Qt::QueuedConnection
    );

    // TODO: maybe register this as a singleton so the QML side understands the type
    mQmlEngine.rootContext()->setContextProperty("Theme", getTheme());

    // this should be removed
    // and a dedicated app should be created just for it
    if (config.isPlayground) {
        mQmlEngine.load(QStringLiteral(":/qt/qml/MacropadCompanion/Playground.qml"));
    } else {
        mQmlEngine.load(QStringLiteral(":/qt/qml/MacropadCompanion/Main.qml"));
    }
}

void App::initTrayIcon() {
    auto trayIcon = new TrayIcon(this);

    QObject::connect(trayIcon, &TrayIcon::activated, this, &App::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::showActionTriggered, this, &App::showWindowRequested);
    QObject::connect(trayIcon, &TrayIcon::quitActionTriggered, qApp, &App::quit);
}

theme::Theme* App::getTheme() {
    if (!mTheme) {
        mTheme = QPointer(theme::Loader::Load(theme::DEFAULT_THEME_NAME));
    }

    return mTheme.data();
}
