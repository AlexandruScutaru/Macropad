#pragma once

#include "app/context/IAppContext.h"
#include "../service/SettingsService.h"
#include "../model/TabsListModel.h"
#include "../model/ThemesListModel.h"

#include <QObject>
#include <QQmlEngine>


namespace settings {
    class SettingsController: public QObject {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(IAppContext* appContext READ appContext WRITE setAppContext REQUIRED)

        Q_PROPERTY(QString currentTheme READ currentTheme NOTIFY currentThemeChanged)
        Q_PROPERTY(TabsListModel* tabsListModel READ tabsListModel NOTIFY tabsListModelChanged)
        Q_PROPERTY(ThemesListModel* themesListModel READ themesListModel NOTIFY themesListModelChanged)
    public:
        explicit SettingsController(QObject* parent = nullptr);
        ~SettingsController();

        QString currentTheme();
        TabsListModel* tabsListModel();
        ThemesListModel* themesListModel();
        
        Q_INVOKABLE void changeTheme(const QString& name);

        IAppContext* appContext() const { return mAppContext; }
        void setAppContext(IAppContext* appContext);

    signals:
        void currentThemeChanged(const QString& name);
        void tabsListModelChanged(TabsListModel* model);
        void themesListModelChanged(ThemesListModel* model);

    private slots:
        void onCurrentThemeChanged(const QString& name);
        void onTabsListChanged(const QStringList& tabs);
        void onThemesListChanged(const std::vector<ThemeEntry>& themes);

    private:
        QString mCurrentTheme;

        TabsListModel* mTabsListModel{ nullptr };
        ThemesListModel* mThemesListModel{ nullptr };

        QPointer<IAppContext> mAppContext{ nullptr };
        QPointer<settings::SettingsService> mSettingsService{ nullptr };

    };
}
