#include "KeysListModel.h"

#include <QDebug>


KeysListModel::KeysListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "KeysListModel::KeysListModel";
}

KeysListModel::~KeysListModel() {
    qDebug() << "KeysListModel::~KeysListModel";
}


QHash<int, QByteArray> KeysListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Round] = "keyIsRound";
    roles[ActionId] = "keyActionId";
    roles[ActionDisplayName] = "keyActionDisplayName";
    roles[ActionIcon] = "keyActionIcon";
    return roles;
}
