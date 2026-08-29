#pragma once

#include "app/context/IAppContext.h"
#include "../model/ActionSectionsListModel.h"
#include "../service/KeypadTypes.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>


class AvailableActionsController : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(IAppContext* appContext WRITE setAppContext REQUIRED)

    Q_PROPERTY(ActionSectionsListModel* model READ model NOTIFY modelChanged)
public:
    explicit AvailableActionsController(QObject* parent = nullptr);
    ~AvailableActionsController();

    ActionSectionsListModel* model();

    void setAppContext(IAppContext* appContext);

signals:
    void modelChanged(ActionSectionsListModel* model);

private slots:
    void onAvailableActionsChanged(const Keypad::AvailableActions& availableActions);

private:
    QPointer<ActionSectionsListModel> mActionSectionsListModel{ nullptr };
    QPointer<KeypadService> mKeypadService{ nullptr };

};
