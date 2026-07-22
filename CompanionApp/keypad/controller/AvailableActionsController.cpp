#include "AvailableActionsController.h"

#include "../KeypadTypes.h"
#include "../model/ActionsListModel.h"

#include <QDebug>


AvailableActionsController::AvailableActionsController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AvailableActionsController::AvailableActionsController";

    mActionSectionsListModel = new ActionSectionsListModel(this);
}

AvailableActionsController::~AvailableActionsController() {
    qDebug() << "AvailableActionsController::~AvailableActionsController";
}


ActionSectionsListModel* AvailableActionsController::model() {
    return mActionSectionsListModel;
}

void AvailableActionsController::onAvailableActionsChanged(const Keypad::AvailableActions& availableActions) {
    if (mActionSectionsListModel) {
        mActionSectionsListModel->deleteLater();
    }

    // deleting this here for now rather than reseting the data
    // as doing that requires additional nested model lifecycle management
    mActionSectionsListModel = new ActionSectionsListModel(this);

    QList<QMap<int, QVariant>> sectionModel;
    sectionModel.reserve(availableActions.sections.size());

    for (const auto& section: availableActions.sections) {
        QMap<int, QVariant> sectionRow;
        sectionRow[ActionSectionsListModel::Name] = section.displayName;
        sectionRow[ActionSectionsListModel::IconName] = section.iconName;

        QList<QMap<int, QVariant>> actionsModel;
        actionsModel.reserve(section.actions.size());
        for (const auto& actionId: section.actions) {
            QMap<int, QVariant> actionRow;
            if (const auto it = availableActions.getAction(actionId); it != std::nullopt) {
                actionRow[ActionsListModel::Id] = it->id;
                actionRow[ActionsListModel::DisplayName] = it->displayName;
                actionRow[ActionsListModel::ToolTip] = it->tooltip;
                actionRow[ActionsListModel::IconName] = it->iconName;
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
    emit modelChanged(mActionSectionsListModel);
}
