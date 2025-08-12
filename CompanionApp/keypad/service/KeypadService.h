#pragma once

#include "../KeypadTypes.h"

#include <QObject>
#include <QPointer>

#include <vector>

class AppSettings;
class ActionSectionsListModel;
class LayerListModel;
class KeysListModel;

class KeypadService : public QObject {
    Q_OBJECT
public:
    KeypadService(const Keypad::Sections& sections, QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadService();

    ActionSectionsListModel* getActionSectionsListModel();
    LayerListModel* getLayerListModel();

    void assignActionRequested(int layer, int key, const QString& action);

private:
    void populateActionsListModel();
    void populateLayersModel(const Keypad::Layers& layers);
    Keypad::Layers loadSavedKeypadConfig();
    std::optional<Keypad::ActionEntry> getActionById(const QString& id);

    QPointer<AppSettings> mAppSettings{ nullptr };
    QPointer<ActionSectionsListModel> mActionSectionsListModel{ nullptr };
    QPointer<LayerListModel> mLayerListModel{ nullptr };
    std::vector<QPointer<KeysListModel>> mKeysLayersModels;

    Keypad::Sections mActionSections;

};
