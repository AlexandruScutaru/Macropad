#pragma once

#include "helpers/ListModel.h"


namespace settings {
    class TabsListModel : public ListModel {
        Q_OBJECT
    public:
        enum ThemeRoles {
            Name = Qt::UserRole + 1,
            Url,
        };

        explicit TabsListModel(QObject* parent = nullptr);
        ~TabsListModel();

    protected:
        QHash<int, QByteArray> roleNames() const override;

    };
}
