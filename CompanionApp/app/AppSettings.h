#pragma once

#include <QObject>
#include <QSize>

#include <memory>


class QSettings;

class AppSettings: public QObject {
    Q_OBJECT
public:
    explicit AppSettings(QObject* parent = nullptr);
    ~AppSettings();

    QSize windowSize();
    void saveWindowSize(const QSize& size);

    QString themeName();
    void saveThemeName(const QString& theme);

    bool navBarExpanded();
    void saveNavBarExpanded(bool expanded);

    QString profileData();
    void saveProfileData(const QString& profile);

private:
    std::unique_ptr<QSettings> getSettings();

    void addLayersAsNeeded(int layer);

    void readWindowState();
    void readTheme();
    void readProfileData();

    QSize mWindowSize;
    QString mThemeName;
    bool mNavBarExpanded{ true };
    QString mProfileData;

};
