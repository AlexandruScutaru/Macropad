#pragma once

#include <QAbstractListModel>


class ListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)

public:
    explicit ListModel(QObject* parent = nullptr);
    virtual ~ListModel() = default;

    Q_INVOKABLE QMap<QString, QVariant> getRow(int index);

    void reset();
    void setData(const QList<QMap<int, QVariant>>& data);
    void updateRow(size_t idx, const QMap<int, QVariant>& row);
    void updateData(const QList<QMap<int, QVariant>>& data);
    int getCount() const;

    virtual QVariant data(const QModelIndex& index, int role) const override;

signals:
    void countChanged(int count);

protected:
    virtual QHash<int, QByteArray> roleNames() const = 0;

    virtual int rowCount(const QModelIndex& parent) const override;

private:
    QList<QMap<int, QVariant>> mData;

};
