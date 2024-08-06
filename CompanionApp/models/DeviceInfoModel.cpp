#include "DeviceInfoModel.h"

#include <QDebug>


DeviceInfoModel::DeviceInfoModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "DeviceInfoModel::DeviceInfoModel";
}

DeviceInfoModel::~DeviceInfoModel() {
    qDebug() << "DeviceInfoModel::~DeviceInfoModel";
}


void DeviceInfoModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void DeviceInfoModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int DeviceInfoModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> DeviceInfoModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[Vid] = "vid";
    roles[Pid] = "pid";
    roles[UsagePage] = "usagePage";
    roles[UsageId] = "usageId";
    roles[Product] = "product";
    roles[Manufacturer] = "manufacturer";
    roles[Serial] = "serial";
    roles[Path] = "path";

    return roles;
}

int DeviceInfoModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant DeviceInfoModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "DeviceInfoModel: role " << role << " no found";
    return {};
}
