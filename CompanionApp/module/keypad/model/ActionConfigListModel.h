#pragma once

#include "helpers/ListModel.h"


class ActionConfigListModel: public ListModel {
    Q_OBJECT
public:
    enum ConfigRoles {
        Type = Qt::UserRole + 1,
        Name,
        DisplayName,
        Tooltip,
        Value,
        WantFolder,
    };

    explicit ActionConfigListModel(QObject* parent = nullptr);
    ~ActionConfigListModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

};
