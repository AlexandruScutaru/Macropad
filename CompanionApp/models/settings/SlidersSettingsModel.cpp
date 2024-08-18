#include "SlidersSettingsModel.h"

#include <QDebug>


SlidersSettingsModel::SlidersSettingsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "SlidersSettingsModel::SlidersSettingsModel";
}

SlidersSettingsModel::~SlidersSettingsModel() {
    qDebug() << "SlidersSettingsModel::~SlidersSettingsModel";
}


void SlidersSettingsModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void SlidersSettingsModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

void SlidersSettingsModel::updateRow(size_t idx, const Row& row) {
    if (idx >= mData.size()) {
        qDebug() << "SlidersSettingsModel: index greater than model data size";
        return;
    }

    mData[idx] = row;
    const QModelIndex modelIndex = index(static_cast<int>(idx));
    emit dataChanged(modelIndex, modelIndex);
}

void SlidersSettingsModel::updateData(const std::vector<Row>& data) {
    if (mData.size() != data.size()) {
        setData(data);
        return;
    }

    const auto checkEquality = [](auto role, const auto& lhs, const auto& rhs) -> bool {
        const auto& lhsRole = lhs.find(role);
        const auto& rhsRole = rhs.find(role);

        if (lhsRole == lhs.end()) return false;
        if (rhsRole == rhs.end()) return false;

        return lhs == rhs;
    };

    for (auto i = 0; i < mData.size(); i++) {
        if (!checkEquality(SlidersSettingsModel::ValueRole, mData[i], data[i]) ||
            !checkEquality(SlidersSettingsModel::MinRole,   mData[i], data[i]) ||
            !checkEquality(SlidersSettingsModel::MaxRole,   mData[i], data[i]))
        {
            updateRow(i, data[i]);
        }
    }
}

int SlidersSettingsModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> SlidersSettingsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[MinRole] = "min";
    roles[MaxRole] = "max";
    roles[ValueRole] = "value";
    return roles;
}

int SlidersSettingsModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant SlidersSettingsModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "SlidersSettingsModel: role " << role << " no found";
    return {};
}
