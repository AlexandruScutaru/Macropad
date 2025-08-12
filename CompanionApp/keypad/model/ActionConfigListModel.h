#pragma once

#include "misc/ListModel.h"


class ActionConfigModel : public ListModel {
    Q_OBJECT
public:
    enum ConfigRoles {
        Type = Qt::UserRole + 1,
        Name,
        ToolTip,
        Value
    };

    explicit ActionConfigModel(QObject* parent = nullptr);
    ~ActionConfigModel() = default;

protected:
    QHash<int, QByteArray> roleNames() const override;

};
