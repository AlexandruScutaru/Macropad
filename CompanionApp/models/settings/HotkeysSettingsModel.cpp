#include "HotkeysSettingsModel.h"

#include <QDebug>


HotkeysSettingsModel::HotkeysSettingsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "HotkeysSettingsModel::HotkeysSettingsModel";
}

HotkeysSettingsModel::~HotkeysSettingsModel() {
    qDebug() << "HotkeysSettingsModel::~HotkeysSettingsModel";
}


void HotkeysSettingsModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void HotkeysSettingsModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int HotkeysSettingsModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> HotkeysSettingsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[KeyRole] = "key";
    return roles;
}

int HotkeysSettingsModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant HotkeysSettingsModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "HotkeysSettingsModel: role " << role << " not found";
    return {};
}
