#pragma once

#include "../model/ActionSectionsListModel.h"
#include "../model/LayerListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

class KeypadService;


class KeypadController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    // I want to create it from the c++ side so I can inject dependencies into it
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit KeypadController(QPointer<KeypadService> service, QObject* parent = nullptr);
    ~KeypadController();

    Q_INVOKABLE ActionSectionsListModel* getActionSectionsListModel();
    Q_INVOKABLE LayerListModel* getLayerListModel();

    Q_INVOKABLE void assignAction(int layer, int key, const QString& actionId);

private:
    QPointer<KeypadService> mService{ nullptr };

};
