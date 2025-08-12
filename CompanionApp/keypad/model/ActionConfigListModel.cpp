#include "ActionConfigListModel.h"

#include <QDebug>


ActionConfigModel::ActionConfigModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> ActionConfigModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Type] = "type",
    roles[Name] = "name",
    roles[ToolTip] = "toolTip";
    roles[Value] = "value";
    return roles;
}
