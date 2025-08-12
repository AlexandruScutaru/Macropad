#pragma once

#include "misc/ListModel.h"


class LayerListModel : public ListModel {
    Q_OBJECT
public:
    enum LayerRoles {
        Color = Qt::UserRole + 1,
        KeysList,
    };

    explicit LayerListModel(QObject* parent = nullptr);
    ~LayerListModel() = default;

protected:
    QHash<int, QByteArray> roleNames() const override;
};
