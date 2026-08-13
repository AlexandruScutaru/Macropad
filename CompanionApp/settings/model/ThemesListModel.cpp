#include "ThemesListModel.h"

#include <QDebug>


using namespace settings;

ThemesListModel::ThemesListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "ThemesListModel::ThemesListModel";
}

ThemesListModel::~ThemesListModel() {
    qDebug() << "ThemesListModel::~ThemesListModel";
}


QHash<int, QByteArray> ThemesListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Colors] = "colors";

    return roles;
}
