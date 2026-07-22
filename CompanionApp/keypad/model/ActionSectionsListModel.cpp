#include "ActionSectionsListModel.h"

#include <QDebug>


ActionSectionsListModel::ActionSectionsListModel(QObject* parent)
    : ListModel(parent)
{
    qDebug() << "ActionSectionsListModel::ActionSectionsListModel";
}

ActionSectionsListModel::~ActionSectionsListModel() {
    qDebug() << "ActionSectionsListModel::~ActionSectionsListModel";
}


QHash<int, QByteArray> ActionSectionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[IconName] = "iconName";
    roles[ActionsList] = "actionList";
    return roles;
}
