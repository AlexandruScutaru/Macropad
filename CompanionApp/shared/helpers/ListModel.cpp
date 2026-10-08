#include "ListModel.h"


ListModel::ListModel(QObject* parent)
    : QAbstractListModel(parent)
{}


void ListModel::reset() {
    beginResetModel();
    mData.clear();
    endResetModel();

    emit countChanged(getCount());
}

void ListModel::setData(const QList<QMap<int, QVariant>>& data) {
    if (!data.size()) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    mData = data;
    endInsertRows();

    emit countChanged(getCount());
}

void ListModel::updateRow(size_t idx, const QMap<int, QVariant>& row) {
    if (idx >= mData.size()) {
        qDebug() << "ListModel: index greater than model data size";
        return;
    }

    mData[idx] = row;
    const QModelIndex modelIndex = index(static_cast<int>(idx));
    emit dataChanged(modelIndex, modelIndex);
}

void ListModel::updateData(const QList<QMap<int, QVariant>>& data) {
    if (mData.size() != data.size()) {
        setData(data);
        return;
    }

    const auto checkEquality = [](auto role, const auto& lhs, const auto& rhs) -> bool {
        const auto& lhsRole = lhs.find(role);
        const auto& rhsRole = rhs.find(role);

        if (lhsRole == lhs.end()) return false;
        if (rhsRole == rhs.end()) return false;

        // TODO: check the case when this is another model
        return lhs == rhs;
    };

    const auto roles = roleNames();
    for (auto i = 0; i < mData.size(); i++) {
        const auto rowEntry = mData[i];
        for (const auto& role: roles.keys()) {
            if (!checkEquality(role, mData[i], data[i])) {
                updateRow(i, data[i]);
                continue;
            }
        }
    }
}

int ListModel::getCount() const {
    return static_cast<int>(mData.size());
}

int ListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return getCount();
}

QVariant ListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    int row = index.row();
    if (row < 0 || row >= mData.size() || role <= Qt::UserRole) {
        return {};
    }

    const auto roles = roleNames();
    const auto rowEntry = mData[row];

    if (const auto& it = rowEntry.find(role); it != rowEntry.end()) {
        return it.value();
    }

    qWarning() << "ListModel: role " << role << " not found";
    return {};
}

QMap<QString, QVariant> ListModel::getRow(int idx) {
    if (idx < 0 || idx >= mData.size()) {
        qWarning() << "ListModel: index " << idx << " out of bounds";
        return {};
    }

    const auto roles = roleNames();
    QHashIterator<int, QByteArray> it(roles);
    QModelIndex modelIndex = index(idx, 0);
    QMap<QString, QVariant> res;
    while (it.hasNext()) {
        it.next();
        QVariant data = modelIndex.data(it.key());
        res[it.value()] = data;
    }

    return res;
}
