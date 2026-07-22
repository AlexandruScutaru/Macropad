#pragma once

#include "misc/ListModel.h"


class ActionsListModel : public ListModel {
    Q_OBJECT
public:
    enum ActionsRoles {
        Id = Qt::UserRole + 1,
        DisplayName,
        ToolTip,
        IconName
    };

    explicit ActionsListModel(QObject* parent = nullptr);
    ~ActionsListModel();

protected:
    QHash<int, QByteArray> roleNames() const override;
};
