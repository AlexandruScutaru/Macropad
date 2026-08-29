#pragma once

#include "helpers/ListModel.h"


namespace settings {
    class ThemesListModel : public ListModel {
        Q_OBJECT
    public:
        enum TabRoles {
            Name = Qt::UserRole + 1,
            Colors,
        };

        explicit ThemesListModel(QObject* parent = nullptr);
        ~ThemesListModel();

    protected:
        QHash<int, QByteArray> roleNames() const override;

    };
}
