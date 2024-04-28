#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include "Macropad.h"

#ifdef NDEBUG
static constexpr bool IS_DEBUG = false;
#else
static constexpr bool IS_DEBUG = true;
#endif

static constexpr auto SHOW_WINDOW_QML_FUNC_NAME = "windowShowRequested";


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/app_icon.png"));
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("basic");

    auto macropad = new Macropad(engine, qApp);

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
    engine.load(QStringLiteral("MacropadCompanion/Main.qml"));

    return app.exec();
}
