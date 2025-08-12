#include "KeysListModel.h"

#include <QDebug>


KeysListModel::KeysListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> KeysListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Round] = "keyIsRound";
    roles[ActionId] = "keyActionId";
    roles[ActionName]= "keyActionName";
    roles[ActionIcon]= "keyActionIcon";
    roles[ActionConfig] = "keyActionConfig";
    return roles;
}
