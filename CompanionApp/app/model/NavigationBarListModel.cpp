#include "NavigationBarListModel.h"

#include <QDebug>


NavigationBarListModel::NavigationBarListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "NavigationBarListModel::NavigationBarListModel";
}

NavigationBarListModel::~NavigationBarListModel() {
    qDebug() << "NavigationBarListModel::~NavigationBarListModel";
}


QHash<int, QByteArray> NavigationBarListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[Type] = "type";
    roles[Name] = "name";
    roles[DisplayName] = "displayName";
    roles[IconSource] = "iconSource";
    roles[Enabled] = "tabEnabled";
    roles[Checkable] = "tabCheckable";
    roles[Animation] = "animation";

    return roles;
}
