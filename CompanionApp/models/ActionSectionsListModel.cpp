#include "ActionSectionsListModel.h"

#include <QDebug>


ActionSectionsListModel::ActionSectionsListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "ActionSectionsListModel::ActionSectionsListModel";
}

ActionSectionsListModel::~ActionSectionsListModel() {
    qDebug() << "ActionSectionsListModel::~ActionSectionsListModel";
}


void ActionSectionsListModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void ActionSectionsListModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int ActionSectionsListModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> ActionSectionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[SectionName] = "sectionName";
    roles[IconName] = "iconName";
    roles[ActionsList] = "actionList";
    return roles;
}

int ActionSectionsListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant ActionSectionsListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    int row = index.row();
    if (row < 0 || row >= mData.size() || role <= Qt::UserRole) {
        return {};
    }

    const auto rowEntry = mData[row];
    if (const auto& it = rowEntry.find(role); it != rowEntry.end()) {
        return it->second;
    }

    qWarning() << "ActionSectionsListModel: role " << role << " not found";
    return {};
}
