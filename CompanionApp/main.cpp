#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSystemTrayIcon>

#include "AudioOutputSwitcher.h"
#include "NativeEventFilter.h"
#include "TrayIcon.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/resources/app_icon.png"));
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("basic");

    auto audioOutputSwitcher = new AudioOutputSwitcher(qApp);
    auto nativeEventFilter = new NativeEventFilter(qApp);

    QObject::connect(nativeEventFilter, &NativeEventFilter::hotKeyTrigerred, audioOutputSwitcher, &AudioOutputSwitcher::onHotKeyTriggered);

    app.installNativeEventFilter(static_cast<QAbstractNativeEventFilter*>(nativeEventFilter));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() {
            qDebug() << "QQmlApplicationEngine::objectCreationFailed";
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [&engine](QObject* obj, const QUrl& objUrl) {
            if (!obj) {
                qDebug() << "QQmlApplicationEngine created object is null (for some reason)";
                QCoreApplication::exit(-2);
            }

            if(engine.rootObjects().isEmpty()) {
                return;
            }

            const auto qmlWindow = engine.rootObjects().constFirst();

            auto trayIcon = new TrayIcon(qApp);
            QObject::connect(trayIcon, &TrayIcon::activated, qApp, [winPtr = QPointer(qmlWindow)](QSystemTrayIcon::ActivationReason reason){
                if (!winPtr) {
                    return;
                }
                if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
                    QMetaObject::invokeMethod(winPtr, "windowShowRequested");
                }
            });

            QObject::connect(trayIcon, &TrayIcon::showActionTriggered, qApp, [winPtr = QPointer(qmlWindow)]() {
                if (!winPtr) {
                    return;
                }
                QMetaObject::invokeMethod(winPtr, "windowShowRequested");
            });

            QObject::connect(trayIcon, &TrayIcon::quitActionTriggered, qApp, &QApplication::quit);
        },
        Qt::QueuedConnection
    );

    engine.load(QStringLiteral("MacropadCompanion/Main.qml"));

    return app.exec();
}
