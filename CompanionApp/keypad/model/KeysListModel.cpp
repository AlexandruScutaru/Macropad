#include "KeysListModel.h"

#include <QDebug>


KeysListModel::KeysListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> KeysListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Round] = "keyIsRound";
    roles[ActionName] = "keyActionName";
    roles[ActionDisplayName] = "keyActionDisplayName";
    roles[ActionIcon] = "keyActionIcon";
    return roles;
}
