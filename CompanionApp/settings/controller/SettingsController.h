#pragma once

#include "../model/TabsListModel.h"
#include "../model/ThemesListModel.h"

#include <QObject>
#include <QQmlEngine>


namespace settings {
    class SettingsController: public QObject {
        Q_OBJECT
        QML_ELEMENT
        QML_UNCREATABLE("Not intended to be created from QML directly")

        Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)

        Q_PROPERTY(TabsListModel* tabsListModel READ tabsListModel NOTIFY tabsListModelChanged)
        Q_PROPERTY(ThemesListModel* themesListModel READ themesListModel NOTIFY themesListModelChanged)

    public:
        explicit SettingsController(QObject* parent = nullptr);
        ~SettingsController();

        Q_INVOKABLE void changeTheme(const QString& name);

        QString currentTheme();
        void setCurrentTheme(const QString& name);

        TabsListModel* tabsListModel();
        ThemesListModel* themesListModel();

    signals:
        void currentThemeChanged(const QString& name);

        void tabsListModelChanged(TabsListModel* model);
        void themesListModelChanged(ThemesListModel* model);

        void changeThemeRequested(const QString& name);

    private:
        TabsListModel* createTabsListModel();
        ThemesListModel* createThemesListModel();

        QString mCurrentTheme;

        TabsListModel* mTabsListModel{ nullptr };
        ThemesListModel* mThemesListModel{ nullptr };

    };
}
