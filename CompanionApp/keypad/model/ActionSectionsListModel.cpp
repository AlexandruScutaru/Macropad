#include "ActionSectionsListModel.h"

#include <QDebug>


ActionSectionsListModel::ActionSectionsListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> ActionSectionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[IconName] = "iconName";
    roles[ActionsList] = "actionList";
    return roles;
}
