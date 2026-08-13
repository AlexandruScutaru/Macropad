#include "TabsListModel.h"

#include <QDebug>


using namespace settings;

TabsListModel::TabsListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "TabsListModel::TabsListModel";
}

TabsListModel::~TabsListModel() {
    qDebug() << "TabsListModel::~TabsListModel";
}


QHash<int, QByteArray> TabsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Url] = "url";

    return roles;
}
