#pragma once

#include "../KeypadTypes.h"
#include "../model/ActionSectionsListModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

namespace Keypad {
    struct AvailableActions;
}


class AvailableActionsController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit AvailableActionsController(const Keypad::AvailableActions& availableActions, QObject* parent = nullptr);
    ~AvailableActionsController();

    Q_INVOKABLE ActionSectionsListModel* getActionSectionsListModel();

private:
    void populateActionsListModel(const Keypad::AvailableActions& availableActions);

    QPointer<ActionSectionsListModel> mActionSectionsListModel{ nullptr };

};
