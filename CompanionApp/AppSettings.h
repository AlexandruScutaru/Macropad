#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSize>

#include <memory>
#include <unordered_map>

class QSettings;

namespace Hotkeys {
    enum class Actions;
}

struct PotentiometerInfo {
    int min{ 0 };
    int max{ 1023 };
};

using Potentiometers =  std::vector<PotentiometerInfo>;


class AppSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")

public:
    explicit AppSettings(QObject* parent = nullptr);
    ~AppSettings();

    QSize windowSize();
    void saveWindowSize(const QSize& size);

    bool navBarExpanded();
    void saveNavBarExpanded(bool expanded);

    Potentiometers potentiometersInfo();
    void savePotentiometersInfo(const Potentiometers& potentiometers);

private:
    std::unique_ptr<QSettings> getSettings();

    void readWindowState();
    void readPotentiometersInfo();

    QSize mWindowSize;
    bool mNavBarExpanded{ true };
    Potentiometers mPotentiometersInfo;

};
