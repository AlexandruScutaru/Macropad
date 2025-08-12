#include "LayerListModel.h"

#include <QDebug>


LayerListModel::LayerListModel(QObject* parent)
    : ListModel(parent)
{}


QHash<int, QByteArray> LayerListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Color] = "color";
    roles[KeysList] = "keysList";
    return roles;
}
