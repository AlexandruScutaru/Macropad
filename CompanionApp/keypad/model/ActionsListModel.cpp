#include "ActionsListModel.h"

#include <QDebug>


ActionsListModel::ActionsListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "ActionsListModel::ActionsListModel";
}

ActionsListModel::~ActionsListModel() {
    qDebug() << "ActionsListModel::~ActionsListModel";
}


QHash<int, QByteArray> ActionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Id] = "id";
    roles[DisplayName] = "displayName";
    roles[IconName] = "iconName";
    roles[ToolTip] = "toolTip";
    return roles;
}
