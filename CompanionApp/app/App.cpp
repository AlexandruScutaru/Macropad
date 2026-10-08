#include "App.h"
#include "Theme.h"
#include "ArgsParser.h"
#include "theming/ThemeLoader.h"
#include "tray/TrayIcon.h"
#include "app/Config.h"
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

    auto args = parseCmdArgs(argc, argv);
    mAppContext = new AppContext(new Config(this), args, this);

    initQmlEngine(args);
    initTrayIcon();
}

App::~App() {
    qDebug() << "App::~App";
}


void App::onChangeThemeRequested(const QString& name) {
    if (mTheme.getName() == name) {
        return;
    }

    theme::Loader::Load(name, mTheme);
}

CmdArgs App::parseCmdArgs(int& argc, char** argv) {
    bool isDebug = false;

#ifndef NDEBUG
    isDebug = true;
#endif

    ArgsParser args(argc, argv, {
        CMD_ARG_SKIP_PHYSICAL_DEVICE,
        CMD_ARG_PLAYGROUND
    });

    const auto isPlayground = isDebug && args.getFlag(CMD_ARG_PLAYGROUND);

    return {
        .isSkipPhysicalDevice = isPlayground || args.getFlag(CMD_ARG_SKIP_PHYSICAL_DEVICE),
        .isPlayground = isPlayground,
    };
}


void App::initQmlEngine(const CmdArgs& args) {
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
    mQmlEngine.rootContext()->setContextProperty("Theme", &mTheme);

    // this should be removed
    // and a dedicated app should be created just for it
    if (args.isPlayground) {
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
