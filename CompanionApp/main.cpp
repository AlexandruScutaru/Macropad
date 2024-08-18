#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include "Config.h"
#include "Macropad.h"

#include "misc/DebugChecker.h"

static constexpr auto SHOW_WINDOW_QML_FUNC_NAME = "windowShowRequested";


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/app_icon.png"));
    QQmlApplicationEngine engine;
    engine.addImportPath("/controls");
    QQuickStyle::setStyle("basic");

    QApplication::setOrganizationName("Macropad");
    QApplication::setApplicationName("Companion");

    auto config = new Config(qApp);
    auto macropad = new Macropad(engine, config, qApp);

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
        &app, [&engine, macropad](QObject* obj, const QUrl& url) {
            if (!obj) {
                qDebug() << "QQmlApplicationEngine created object is null (for some reason) for " << url;
                QCoreApplication::exit(-2);
            }

            if(engine.rootObjects().isEmpty()) {
                return;
            }
            const auto qmlWindow = engine.rootObjects().constFirst();

            macropad->onInitialized(IS_DEBUG);
            QObject::connect(macropad, &Macropad::showWindowRequested, qApp, [winPtr = QPointer(qmlWindow)]() {
                if (!winPtr) {
                    return;
                }

                QMetaObject::invokeMethod(winPtr, SHOW_WINDOW_QML_FUNC_NAME);
            });
        },
        Qt::QueuedConnection
    );

    engine.rootContext()->setContextProperty("isDebugInstance", IS_DEBUG);
    engine.rootContext()->setContextProperty("MacroPad", macropad);
    engine.load(QStringLiteral(":/qt/qml/MacropadCompanion/Main.qml"));

    return app.exec();
}
