#include "SlidersModel.h"

#include <QDebug>


SlidersModel::SlidersModel(QObject* parent)
    : QAbstractListModel(parent)
{
    qDebug() << "SlidersModel::SlidersModel";
}

SlidersModel::~SlidersModel() {
    qDebug() << "SlidersModel::~SlidersModel";
}


void SlidersModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void SlidersModel::setData(const std::vector<Row>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

void SlidersModel::updateRow(size_t idx, const Row& row) {
    if (idx >= mData.size()) {
        qDebug() << "SlidersModel: index greater than model data size";
        return;
    }

    mData[idx] = row;
    const QModelIndex modelIndex = index(static_cast<int>(idx));
    emit dataChanged(modelIndex, modelIndex);
}

void SlidersModel::updateData(const std::vector<Row>& data) {
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
        if (!checkEquality(SlidersModel::ValueRole, mData[i], data[i])) {
            updateRow(i, data[i]);
        }
    }
}

int SlidersModel::getCount() const {
    return static_cast<int>(mData.size());
}

QHash<int, QByteArray> SlidersModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ValueRole] = "value";
    return roles;
}

int SlidersModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant SlidersModel::data(const QModelIndex& index, int role) const {
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

    qWarning() << "SlidersModel: role " << role << " not found";
    return {};
}
