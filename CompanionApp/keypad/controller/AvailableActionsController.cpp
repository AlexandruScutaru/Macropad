#include "AvailableActionsController.h"

#include "../KeypadTypes.h"
#include "../model/ActionsListModel.h"

#include <QDebug>


AvailableActionsController::AvailableActionsController(const Keypad::AvailableActions& availableActions, QObject* parent)
    : QObject(parent)
    , mActionSectionsListModel(new ActionSectionsListModel(this))
{
    qDebug() << "AvailableActionsController::AvailableActionsController";
    populateActionsListModel(availableActions);
}

AvailableActionsController::~AvailableActionsController() {
    qDebug() << "AvailableActionsController::~AvailableActionsController";
}


ActionSectionsListModel* AvailableActionsController::getActionSectionsListModel() {
    return mActionSectionsListModel;
}

void AvailableActionsController::populateActionsListModel(const Keypad::AvailableActions& availableActions) {
    QList<QMap<int, QVariant>> sectionModel;
    sectionModel.reserve(availableActions.sections.size());

    for (const auto& section: availableActions.sections) {
        QMap<int, QVariant> sectionRow;
        sectionRow[ActionSectionsListModel::Name] = section.displayName;
        sectionRow[ActionSectionsListModel::IconName] = section.iconName;

        QList<QMap<int, QVariant>> actionsModel;
        actionsModel.reserve(section.actions.size());
        for (const auto& actionName: section.actions) {
            QMap<int, QVariant> actionRow;
            if (const auto action = availableActions.getAction(actionName); action != std::nullopt) {
                actionRow[ActionsListModel::Name] = action->name;
                actionRow[ActionsListModel::DisplayName] = action->displayName;
                actionRow[ActionsListModel::ToolTip] = action->tooltip;
                actionRow[ActionsListModel::IconName] = action->icon;
            }
            actionsModel.push_back(actionRow);
        }

        // mind this lives however long the parent model does
        // when at some point the model data is reset but the parent is not de-allocated
        // these sub-models will need to be manually dealt with
        const auto actionsListModel = new ActionsListModel(mActionSectionsListModel);
        actionsListModel->setData(actionsModel);
        sectionRow[ActionSectionsListModel::ActionsList] = QVariant::fromValue(actionsListModel);
        sectionModel.push_back(sectionRow);
    }

    mActionSectionsListModel->setData(sectionModel);
}
