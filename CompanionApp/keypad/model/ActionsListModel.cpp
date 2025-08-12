#include "ActionsListModel.h"

#include <QDebug>


ActionsListModel::ActionsListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> ActionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Id] = "actionId";
    roles[Name] = "name";
    roles[IconName] = "iconName";
    roles[ToolTip] = "toolTip";
    return roles;
}
