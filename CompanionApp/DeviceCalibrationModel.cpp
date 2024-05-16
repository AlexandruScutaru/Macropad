#include "DeviceCalibrationModel.h"

#include <QDebug>


DeviceCalibrationModel::DeviceCalibrationModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "DeviceCalibrationModel::DeviceCalibrationModel";
}

DeviceCalibrationModel::~DeviceCalibrationModel() {
    qDebug() << "DeviceCalibrationModel::~DeviceCalibrationModel";
}


void DeviceCalibrationModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void DeviceCalibrationModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

int DeviceCalibrationModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> DeviceCalibrationModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[MinRole] = "min";
    roles[MaxRole] = "max";
    roles[ValueRole] = "value";
    return roles;
}

int DeviceCalibrationModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant DeviceCalibrationModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "DeviceCalibrationModel: role " << role << " no found";
    return {};
}
