#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSize>

#include <memory>
#include <unordered_map>


class QSettings;

class AppSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")

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
