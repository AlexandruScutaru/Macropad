#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSystemTrayIcon>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>

#include "AudioOutputSwitcher.h"
#include "NativeEventFilter.h"
#include "TrayIcon.h"

#include "HidWrapper.h"
#include "DevHelperController.h"

#ifdef NDEBUG
static constexpr bool IS_DEBUG = false;
#else
static constexpr bool IS_DEBUG = true;
#endif


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

            if (IS_DEBUG) {
                if (auto devHelperViewContainer = qmlWindow->findChild<QObject*>("devHelperViewContainer"); devHelperViewContainer) {
                    QQmlComponent devView(&engine, QStringLiteral("MacropadCompanion/DevHelperView.qml"));
                    auto devHelperController = new DevHelperController(qApp);

                    auto component = devView.createWithInitialProperties(QVariantMap{{ "controller", QVariant::fromValue<DevHelperController*>(devHelperController) }});
                    QQuickItem* item = qobject_cast<QQuickItem*>(component);
                    item->setParentItem(qobject_cast<QQuickItem*>(devHelperViewContainer));
                }
            }

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

    engine.rootContext()->setContextProperty("isDebugInstance", IS_DEBUG);
    engine.load(QStringLiteral("MacropadCompanion/Main.qml"));

    auto hid = new HidWrapper(qApp);
    if (hid->init()) {
        if (hid->openDevice(0xFEED, 0xB00B)) {
            //while(hid->recv()) {}
        }
    }

    return app.exec();
}
