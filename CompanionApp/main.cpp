#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(ControlsPlugin)

#include "AppSettings.h"
#include "Macropad.h"
#include "misc/CmdArgs.h"
#include "misc/DebugChecker.h"

static constexpr auto CMD_ARG_SKIP_PHYSICAL_DEVICE = "--skipPhysicalDevice";
static constexpr auto CMD_ARG_PLAYGROUND = "--playground";

static constexpr auto SHOW_WINDOW_QML_FUNC_NAME = "windowShowRequested";


int main(int argc, char *argv[]) {
    CmdArgs cmdArgs(argc, argv, {
        CMD_ARG_SKIP_PHYSICAL_DEVICE,
        CMD_ARG_PLAYGROUND
    });

    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/app_icon.png"));
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("basic");

    QApplication::setOrganizationName("Macropad");
    QApplication::setApplicationName("Companion");

    auto appSettings = new AppSettings(qApp);
    auto macropad = new Macropad(engine, appSettings, qApp);

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, [](const QUrl &url) {
            qDebug() << "QQmlApplicationEngine::objectCreationFailed for " << url;
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [&engine, macropad, &cmdArgs](QObject* obj, const QUrl& url) {
            if (!obj) {
                qDebug() << "QQmlApplicationEngine created object is null (for some reason) for " << url;
                QCoreApplication::exit(-2);
            }

            if(engine.rootObjects().isEmpty()) {
                return;
            }
            const auto qmlWindow = engine.rootObjects().constFirst();

            QObject::connect(macropad, &Macropad::showWindowRequested, qApp, [winPtr = QPointer(qmlWindow)]() {
                if (!winPtr) {
                    return;
                }

                QMetaObject::invokeMethod(winPtr, SHOW_WINDOW_QML_FUNC_NAME);
            });

            QObject::connect(macropad, &Macropad::themeChanged, qApp, [&engine](Theme* theme) {
                engine.rootContext()->setContextProperty("Theme", theme);
            });

            macropad->onInitialized({
                .isDebug = IS_DEBUG,
                .isSkipPhysicalDevice = cmdArgs.getFlag(CMD_ARG_SKIP_PHYSICAL_DEVICE),
                .isPlayground = cmdArgs.getFlag(CMD_ARG_PLAYGROUND)
            });
        },
        Qt::QueuedConnection
    );

    engine.rootContext()->setContextProperty("MacroPad", macropad);
    engine.rootContext()->setContextProperty("Theme", macropad->getTheme());
    engine.load(QStringLiteral(":/qt/qml/MacropadCompanion/Main.qml"));

    return app.exec();
}
