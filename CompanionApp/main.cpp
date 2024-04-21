#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "AudioOutputSwitcher.h"
#include "NativeEventFilter.h"


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("basic");

    QScopedPointer<AudioOutputSwitcher> audioOutputSwitcher(new AudioOutputSwitcher);

    auto nativeEventFilter = new NativeEventFilter;
    QObject::connect(nativeEventFilter, &NativeEventFilter::hotKeyTrigerred, audioOutputSwitcher.get(), &AudioOutputSwitcher::onHotKeyTriggered);

    app.installNativeEventFilter(static_cast<QAbstractNativeEventFilter*>(nativeEventFilter));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(QStringLiteral("MacropadCompanion/Main.qml"));

    return app.exec();
}
