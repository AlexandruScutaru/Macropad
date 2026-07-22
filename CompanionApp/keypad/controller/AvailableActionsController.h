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
    Q_PROPERTY(ActionSectionsListModel* model READ model NOTIFY modelChanged)
public:
    explicit AvailableActionsController(QObject* parent = nullptr);
    ~AvailableActionsController();

    ActionSectionsListModel* model();

signals:
    void modelChanged(ActionSectionsListModel* model);

public slots:
    void onAvailableActionsChanged(const Keypad::AvailableActions& availableActions);

private:
    QPointer<ActionSectionsListModel> mActionSectionsListModel{ nullptr };

};
