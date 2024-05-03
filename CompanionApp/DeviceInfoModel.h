#pragma once

#include <QAbstractListModel>
#include <QString>

#include <vector>
#include <unordered_map>


class DeviceInfoModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)
public:
    using Row = std::unordered_map<int, QVariant>;

    enum DeviceInfoRoles {
        Vid = Qt::UserRole + 1,
        Pid,
        UsagePage,
        UsageId,
        Product,
        Manufacturer,
        Serial,
        Path
    };

    explicit DeviceInfoModel(QObject* parent = nullptr);
    ~DeviceInfoModel();

    void reset();
    void setData(const std::vector<Row>& data);

    int getCount() const;

signals:
    void countChanged(int count);

protected:
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

private:
    std::vector<Row> mData;

};
