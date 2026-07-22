#pragma once

#include "misc/ListModel.h"


class ActionConfigListModel : public ListModel {
    Q_OBJECT
public:
    enum ConfigRoles {
        Type = Qt::UserRole + 1,
        Name,
        DisplayName,
        Tooltip,
        Value
    };

    explicit ActionConfigListModel(QObject* parent = nullptr);
    ~ActionConfigListModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

};
