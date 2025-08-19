#pragma once

#include "../model/KeysListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include <tuple>

namespace Keypad {
    struct Action;
    struct Layer;
    struct Profile;
}

class KeypadController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")

    Q_PROPERTY(int layerCount READ layerCount NOTIFY layerCountChanged)
    Q_PROPERTY(int currentLayer READ currentLayer WRITE setCurrentLayer NOTIFY currentLayerChanged)
    Q_PROPERTY(QString layerColor READ layerColor WRITE setLayerColor NOTIFY layerColorChanged)
    Q_PROPERTY(KeysListModel* model READ model NOTIFY modelChanged)

public:
    explicit KeypadController(QObject* parent = nullptr);
    ~KeypadController();

    Q_INVOKABLE void assignAction(int key, const QString& actionName);
    Q_INVOKABLE void onKeySelected(int key);
    Q_INVOKABLE void onKeyTriggered(int key);

    int layerCount();
    int currentLayer();
    QString layerColor();
    KeysListModel* model();

    void setCurrentLayer(int layer);
    void setLayerColor(const QString& color);

signals:
    void layerCountChanged(int count);
    void currentLayerChanged(int layer);
    void layerColorChanged(const QString& color);
    void modelChanged(KeysListModel* model);

    void actionAssignRequested(int layer, int key, const QString& actionName);
    void keySelected(int layer, int key);
    void keyTriggered(int layer, int key);

public slots:
    void onProfileChanged(const Keypad::Profile& profile);
    void onActionAssigned(int layer, int key, const Keypad::Action& action);

private:
    KeysListModel* createKeysListModels(const Keypad::Layer& layer);
    bool isIndexInBounds(int index) const;
    void handleLayerChange(int newLayer);

    using LayerData = std::tuple<QString, QPointer<KeysListModel>>;
    std::vector<LayerData> mLayers;

    int mLayerCount{ 0 };
    int mCurrentLayer{ 0 };
    QString mLayerColor{ "transparent" };
    KeysListModel* mCurrentModel{ nullptr };

};
