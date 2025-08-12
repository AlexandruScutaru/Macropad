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
using Potentiometers = std::vector<PotentiometerInfo>;

struct KeyInfo {
    QString id;
    std::vector<QString> options;
};
using Keys = std::array<KeyInfo, 9>;

struct LayerInfo {
    QString color{ "transparent" };
    Keys keys;
};
using Layers = std::vector<LayerInfo>;

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

    Layers layersInfo();
    void saveKeyAssignment(int layer, int key, const QString& action);
    void saveLayersInfo(const Layers& layers);

private:
    std::unique_ptr<QSettings> getSettings();

    void addLayersAsNeeded(int layer);

    void readWindowState();
    void readPotentiometersInfo();
    void readLayersInfo();

    QSize mWindowSize;
    bool mNavBarExpanded{ true };
    Potentiometers mPotentiometersInfo;
    Layers mLayersInfo;

};
