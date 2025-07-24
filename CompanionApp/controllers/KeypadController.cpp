#include "KeypadController.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

#include <vector>

KeypadController::KeypadController(QObject* parent)
    : QObject(parent)
    , mActionSectionsListModel(new ActionSectionsListModel(this))
{
    qDebug() << "KeypadController::KeypadController";

    // temporary dummy data to populate the UI for the time being
    setAvailableActions({
        {
            .sectionName = "A Section", .iconName = "keys_icon.svg", .actions = {
                { .id =  1, .actionName = "My action",               .toolTip = "Tool Tip", .iconName = "keys_icon.svg" },
                { .id =  2, .actionName = "Some action",             .toolTip = "Tool Tip", .iconName = "keys_icon.svg" },
                { .id =  3, .actionName = "Oh damn another action",  .toolTip = "Tool Tip", .iconName = "keys_icon.svg" },
                { .id =  4, .actionName = "Some other action",       .toolTip = "Tool Tip", .iconName = "keys_icon.svg" },
                { .id =  5, .actionName = "THE action",              .toolTip = "Tool Tip", .iconName = "keys_icon.svg" }
            }
        },
        {
            .sectionName = "Another section", .iconName = "sliders_icon.svg", .actions = {
                { .id =  6, .actionName = "Some action",             .toolTip = "Tool Tip", .iconName = "sliders_icon.svg" },
                { .id =  7, .actionName = "My action",               .toolTip = "Tool Tip", .iconName = "sliders_icon.svg" }
            }
        },
        {
            .sectionName = "Very important section", .iconName = "settings_icon.svg", .actions = {
                { .id =  8, .actionName = "My action",               .toolTip = "Tool Tip", .iconName = "settings_icon.svg" },
                { .id =  9, .actionName = "Some action",             .toolTip = "Tool Tip", .iconName = "settings_icon.svg" },
                { .id = 10, .actionName = "Oh damn another action",  .toolTip = "Tool Tip", .iconName = "settings_icon.svg" },
                { .id = 11, .actionName = "Some other action",       .toolTip = "Tool Tip", .iconName = "settings_icon.svg" },
                { .id = 12, .actionName = "THE action",              .toolTip = "Tool Tip", .iconName = "settings_icon.svg" }
            }
        },
        {
            .sectionName = "Other section", .iconName = "close_icon.svg", .actions = {
                { .id = 13, .actionName = "My action",               .toolTip = "Tool Tip", .iconName = "close_icon.svg" },
                { .id = 14, .actionName = "Some action",             .toolTip = "Tool Tip", .iconName = "close_icon.svg" },
                { .id = 15, .actionName = "Oh damn another action",  .toolTip = "Tool Tip", .iconName = "close_icon.svg" }
            }
        }
    });
}

KeypadController::~KeypadController() {
    qDebug() << "KeypadController::~KeypadController";
}


void KeypadController::setAvailableActions(const ActionSections& actionSections) {
    std::vector<ActionSectionsListModel::Row> sectionModel;
    sectionModel.reserve(actionSections.size());

    for (const auto& section: actionSections) {
        ActionSectionsListModel::Row sectionRow;
        sectionRow[ActionSectionsListModel::SectionName] = QString::fromStdString(section.sectionName);
        sectionRow[ActionSectionsListModel::IconName] = QString::fromStdString(section.iconName);

        std::vector<ActionsListModel::Row> actionsModel;
        actionsModel.reserve(section.actions.size());
        for (const auto& action: section.actions) {
            ActionsListModel::Row actionRow;
            actionRow[ActionsListModel::ActionId] = action.id;
            actionRow[ActionsListModel::ActionName] = QString::fromStdString(action.actionName);
            actionRow[ActionsListModel::ActionToolTip] = QString::fromStdString(action.toolTip);
            actionRow[ActionsListModel::IconName] = QString::fromStdString(action.iconName);
            actionsModel.push_back(actionRow);
        }

        const auto actionsListModel = new ActionsListModel(this);
        actionsListModel->setData(actionsModel);
        sectionRow[ActionSectionsListModel::ActionsList] = QVariant::fromValue(actionsListModel);
        sectionModel.push_back(sectionRow);
    }

    mActionSectionsListModel->setData(sectionModel);
}

ActionSectionsListModel* KeypadController::getActionSectionsListModel() {
    return mActionSectionsListModel;
}
