#pragma once

#include "app/context/IAppContext.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>
#include <QSize>


namespace settings {
    class AppController: public QObject {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(IAppContext* appContext WRITE setAppContext REQUIRED)

        Q_PROPERTY(QSize windowSize READ windowSize NOTIFY windowSizeChanged)
        Q_PROPERTY(bool navBarExpanded READ navBarExpanded NOTIFY navBarExpandedChanged)
    public:
        explicit AppController(QObject* parent = nullptr);
        ~AppController();

        QSize windowSize();
        Q_INVOKABLE void setWindowSize(const QSize& size);

        bool navBarExpanded();
        Q_INVOKABLE void setNavBarExpanded(bool expanded);

        Q_INVOKABLE void connectToDevice();

        void setAppContext(IAppContext* appContext);

    signals:
        void windowSizeChanged(const QSize& size);
        void navBarExpandedChanged(bool expanded);

        void showWindowRequested();
        void deviceConnected();
        void deviceNotFound();

    private:
        QSize mWindowSize;
        bool mNavBarExpanded{ false };

        QPointer<settings::SettingsService> mSettingsService{ nullptr };
        QPointer<hid::DeviceService> mHidDeviceService{ nullptr };

    };
}
