#pragma once

#include "misc/ListModel.h"


class ActionSectionsListModel : public ListModel {
    Q_OBJECT
public:
    enum ActionsRoles {
        Name = Qt::UserRole + 1,
        IconName,
        ActionsList
    };

    explicit ActionSectionsListModel(QObject* parent = nullptr);
    ~ActionSectionsListModel() = default;

protected:
    QHash<int, QByteArray> roleNames() const override;
};
