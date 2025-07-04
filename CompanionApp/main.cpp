#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_QML_PLUGIN(ControlsPlugin)

#include "AppSettings.h"
#include "Macropad.h"

#include "misc/DebugChecker.h"

#include <algorithm>

static constexpr auto SHOW_WINDOW_QML_FUNC_NAME = "windowShowRequested";

bool isArgumentPresent(char** begin, char** end, const std::string& argument);


int main(int argc, char *argv[]) {
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
        &app, [&engine, macropad, argv, argc](QObject* obj, const QUrl& url) {
            if (!obj) {
                qDebug() << "QQmlApplicationEngine created object is null (for some reason) for " << url;
                QCoreApplication::exit(-2);
            }

            if(engine.rootObjects().isEmpty()) {
                return;
            }
            const auto qmlWindow = engine.rootObjects().constFirst();

            const std::string skipPhysicalDeviceArgName = "--skipPhysicalDevice";
            qDebug() << "Argument " << QString::fromStdString(skipPhysicalDeviceArgName) << ": " << isArgumentPresent(argv, argv + argc, skipPhysicalDeviceArgName);
            macropad->onInitialized({
                .isDebug = IS_DEBUG,
                .isSkipPhysicalDevice = isArgumentPresent(argv, argv + argc, skipPhysicalDeviceArgName)
            });
            QObject::connect(macropad, &Macropad::showWindowRequested, qApp, [winPtr = QPointer(qmlWindow)]() {
                if (!winPtr) {
                    return;
                }

                QMetaObject::invokeMethod(winPtr, SHOW_WINDOW_QML_FUNC_NAME);
            });
        },
        Qt::QueuedConnection
    );

    engine.rootContext()->setContextProperty("isDebugInstance", QVariant(IS_DEBUG));
    engine.rootContext()->setContextProperty("MacroPad", macropad);
    engine.load(QStringLiteral(":/qt/qml/MacropadCompanion/Main.qml"));

    return app.exec();
}

bool isArgumentPresent(char** begin, char** end, const std::string& argument) {
    return std::find(begin, end, argument) != end;
}
