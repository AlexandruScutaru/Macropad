#include "ActionConfigListModel.h"

#include <QDebug>


ActionConfigListModel::ActionConfigListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> ActionConfigListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Type] = "type",
    roles[Name] = "name",
    roles[DisplayName] = "displayName",
    roles[Tooltip] = "tooltip";
    roles[Value] = "value";
    return roles;
}
