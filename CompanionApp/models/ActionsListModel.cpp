#include "ActionsListModel.h"

#include <QDebug>


ActionsListModel::ActionsListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "ActionsListModel::ActionsListModel";
}

ActionsListModel::~ActionsListModel() {
    qDebug() << "ActionsListModel::~ActionsListModel";
}


void ActionsListModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void ActionsListModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int ActionsListModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> ActionsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ActionId] = "actionId";
    roles[ActionName] = "actionName";
    roles[IconName] = "iconName";
    return roles;
}

int ActionsListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant ActionsListModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "ActionsListModel: role " << role << " not found";
    return {};
}
