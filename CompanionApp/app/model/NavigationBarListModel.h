#pragma once

#include "shared/helpers/ListModel.h"


class NavigationBarListModel : public ListModel {
    Q_OBJECT
public:
    enum NavBarRoles {
        Type = Qt::UserRole + 1,
        Name,
        DisplayName,
        IconSource,
        Enabled,
        Checkable,
        Animation,
    };

    explicit NavigationBarListModel(QObject* parent = nullptr);
    ~NavigationBarListModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

};
