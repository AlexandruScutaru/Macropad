#pragma once

#include <QList>
#include <QPointer>
#include <QObject>
#include <QString>
#include <QSize>

#include <vector>


class Config;
namespace theme {
    class Theme;
}

namespace settings {
    using ThemeEntry = std::tuple<QString, QVariant>;

    class SettingsService: public QObject {
        Q_OBJECT
    public:
        explicit SettingsService(Config* config, QObject* parent = nullptr);
        ~SettingsService();

        void setNavBarExpanded(bool expanded);
        bool getNavBarExpanded() const;

        void setWindowSize(const QSize& size);
        QSize getWindowSize() const;

        QString getCurrentTheme() const;
        QStringList getAvailableTabs() const;
        std::vector<ThemeEntry> getAvailableThemes() const;
        void changeTheme(const QString& name);

    signals:
        void showWindowRequested();
        void changeThemeRequested(const QString& name);

    private:
        QString mCurrentTheme;
        QPointer<Config> mConfig{ nullptr };
        std::vector<ThemeEntry> mAvailableThemes;

    };
}
