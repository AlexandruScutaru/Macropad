#pragma once

#include "misc/ListModel.h"


class KeysListModel : public ListModel {
    Q_OBJECT
public:
    enum KeysRoles {
        Round = Qt::UserRole + 1,
        ActionId,
        ActionDisplayName,
        ActionIcon
    };

    explicit KeysListModel(QObject* parent = nullptr);
    ~KeysListModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

};
