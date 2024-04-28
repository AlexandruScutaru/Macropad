#include "DevHelperModel.h"

#include <QDebug>


DevHelperModel::DevHelperModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "DevHelperModel::DevHelperModel";
}

DevHelperModel::~DevHelperModel() {
    qDebug() << "DevHelperModel::~DevHelperModel";
}


void DevHelperModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void DevHelperModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int DevHelperModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> DevHelperModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ValueRole] = "value";
    return roles;
}

int DevHelperModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant DevHelperModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "DevHelperModel: role " << role << " no found";
    return {};
}
