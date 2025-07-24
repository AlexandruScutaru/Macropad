#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include "models/ActionsListModel.h"
#include "models/ActionSectionsListModel.h"

#include <string>
#include <vector>

struct ActionEntry {
    int id;
    std::string actionName;
    std::string toolTip;
    std::string iconName;
};
using Actions = std::vector<ActionEntry>;

struct ActionSection {
    std::string sectionName;
    std::string iconName;
    Actions actions;
};
using ActionSections = std::vector<ActionSection>;

class KeypadController : public QObject {
    Q_OBJECT
    QML_ELEMENT

    // I want to create it from the c++ side so I can inject dependencies into it
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit KeypadController(QObject* parent = nullptr);
    ~KeypadController();

    Q_INVOKABLE ActionSectionsListModel* getActionSectionsListModel();

    void setAvailableActions(const ActionSections& actionSections);

signals:

private slots:

private:
    QPointer<ActionSectionsListModel> mActionSectionsListModel{ nullptr };

};
