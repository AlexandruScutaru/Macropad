#pragma once

#include "app/context/IAppContext.h"
#include "../model/NavigationBarListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>
#include <QSize>


namespace settings {
    class AppController: public QObject {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(IAppContext* appContext READ appContext WRITE setAppContext REQUIRED)

        Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
        Q_PROPERTY(bool navBarExpanded READ navBarExpanded WRITE setNavBarExpanded NOTIFY navBarExpandedChanged)
        Q_PROPERTY(NavigationBarListModel* navBarModel READ navBarModel NOTIFY navBarModelChanged)

    public:
        explicit AppController(QObject* parent = nullptr);
        ~AppController();

        Q_INVOKABLE void connectToDevice();
        Q_INVOKABLE void navigationRequested(const QString& tabName);

        IAppContext* appContext() const { return mAppContext; }
        void setAppContext(IAppContext* appContext);

        QSize windowSize();
        void setWindowSize(const QSize& size);

        bool navBarExpanded();
        void setNavBarExpanded(bool expanded);

        NavigationBarListModel* navBarModel();

    signals:
        void windowSizeChanged(const QSize& size);
        void navBarExpandedChanged(bool expanded);
        void navBarModelChanged(NavigationBarListModel* navBarModel);

        void showWindowRequested();
        void deviceConnected();
        void deviceNotFound();

        void openSettings();
        void switchTab(int index, const QString& name);

    private:
        void setTabButtonState(const QString& name, bool enabled);
        void createNavBarModel();

        QSize mWindowSize;
        bool mNavBarExpanded{ false };

        QPointer<IAppContext> mAppContext{ nullptr };
        QPointer<settings::SettingsService> mSettingsService{ nullptr };
        QPointer<hid::DeviceService> mHidDeviceService{ nullptr };
        QPointer<NavigationBarListModel> mNavBarModel{ nullptr };

    };
}
