#include "KeypadService.h"
#include "AppSettings.h"
#include "../model/ActionSectionsListModel.h"
#include "../model/ActionsListModel.h"
#include "../model/LayerListModel.h"
#include "../model/KeysListModel.h"

#include <QDebug>


KeypadService::KeypadService(const Keypad::AvailableActions& actions, QPointer<AppSettings> appSettings, QObject* parent)
    : QObject(parent)
    , mAppSettings(appSettings)
    , mActionSectionsListModel(new ActionSectionsListModel(this))
    , mLayerListModel(new LayerListModel(this))
{
    qDebug() << "KeypadService::KeypadService";

    mActionsMap = std::get<0>(actions);
    mActionSections = std::get<1>(actions);

    populateActionsListModel();
    populateLayersModel(loadSavedKeypadConfig());
}

KeypadService::~KeypadService() {
    qDebug() << "KeypadService::~KeypadService";
}


ActionSectionsListModel* KeypadService::getActionSectionsListModel() {
    return mActionSectionsListModel;
}

LayerListModel* KeypadService::getLayerListModel() {
    return mLayerListModel;
}

void KeypadService::assignActionRequested(int layer, int key, const QString& action) {
    mAppSettings->saveKeyAssignment(layer, key, action);

    // update model to reflect changes
    QMap<int, QVariant> keyRow;
    if (const auto actionInfo = getAction(action)) {
        keyRow[KeysListModel::Round] = key == 0;
        keyRow[KeysListModel::ActionId] = actionInfo->id;
        keyRow[KeysListModel::ActionName] = actionInfo->name;
        keyRow[KeysListModel::ActionIcon] = actionInfo->tooltip;
        keyRow[KeysListModel::ActionConfig] = {};
    }
    mKeysLayersModels[layer]->updateRow(key, keyRow);
}

void KeypadService::populateActionsListModel() {
    QList<QMap<int, QVariant>> sectionModel;
    sectionModel.reserve(mActionSections.size());

    for (const auto& section: mActionSections) {
        QMap<int, QVariant> sectionRow;
        sectionRow[ActionSectionsListModel::Name] = section.name;
        sectionRow[ActionSectionsListModel::IconName] = section.iconName;

        QList<QMap<int, QVariant>> actionsModel;
        actionsModel.reserve(section.actions.size());
        for (const auto& actionId: section.actions) {
            QMap<int, QVariant> actionRow;
            if (const auto action = getAction(actionId); action != std::nullopt) {
                actionRow[ActionsListModel::Id] = action->id;
                actionRow[ActionsListModel::Name] = action->name;
                actionRow[ActionsListModel::ToolTip] = action->tooltip;
                actionRow[ActionsListModel::IconName] = action->icon;
            }
            actionsModel.push_back(actionRow);
        }

        // mind this lives however long the parent model does
        // when at some point the model data is reset but the parent is not de-allocated
        // these sub-models will need to be manually dealt with
        const auto actionsListModel = new ActionsListModel(this);
        actionsListModel->setData(actionsModel);
        sectionRow[ActionSectionsListModel::ActionsList] = QVariant::fromValue(actionsListModel);
        sectionModel.push_back(sectionRow);
    }

    mActionSectionsListModel->setData(sectionModel);
}

void KeypadService::populateLayersModel(const Keypad::Layers& layers) {
    QList<QMap<int, QVariant>> layersModel;
    layersModel.reserve(layers.size());

    for (auto& keysModel: mKeysLayersModels) {
        if (keysModel) {
            keysModel->deleteLater();
        }
    }
    mKeysLayersModels.clear();

    mKeysLayersModels.reserve(layers.size());
    for (int layer = 0; layer < layers.size(); layer++) {
        QMap<int, QVariant> layerRow;
        layerRow[LayerListModel::Color] = "red";
        QList<QMap<int, QVariant>> keysModel;
        for (int key = 0; key < layers[layer].actions.size(); key++) {
            QMap<int, QVariant> keyRow;
            keyRow[KeysListModel::Round] = key == 0;
            keyRow[KeysListModel::ActionId] = layers[layer].actions[key].id;
            keyRow[KeysListModel::ActionName] = layers[layer].actions[key].name;
            keyRow[KeysListModel::ActionIcon] = layers[layer].actions[key].icon;
            keyRow[KeysListModel::ActionConfig] = {};
            keysModel.push_back(keyRow);
        }
        const auto keysListModel = new KeysListModel(this);
        mKeysLayersModels.push_back(QPointer(keysListModel));
        keysListModel->setData(keysModel);
        layerRow[LayerListModel::KeysList] = QVariant::fromValue(keysListModel);
        layersModel.push_back(layerRow);
    }

    mLayerListModel->setData(layersModel);
}

Keypad::Layers KeypadService::loadSavedKeypadConfig() {
    const auto savedConfig = mAppSettings->layersInfo();

    Keypad::Layers layers;
    layers.reserve(savedConfig.size());

    for (const auto& savedLayer: savedConfig) {
        Keypad::Layer layer;
        layer.color = savedLayer.color;
        layer.actions.reserve(savedLayer.keys.size());
        for (const auto& savedKey: savedLayer.keys) {
            Keypad::Action action;
            if (const auto actionInfo = getAction(savedKey.id); actionInfo != std::nullopt) {
                action.id = actionInfo->id;
                action.name = actionInfo->name;
                action.icon = actionInfo->icon;
            }
            layer.actions.push_back(action);
        }
        layers.push_back(layer);
    }

    return layers;
}

std::optional<Keypad::Action> KeypadService::getAction(const QString& id) {
    if (const auto& action = mActionsMap.find(id); action != mActionsMap.end()) {
        return action->second;
    }

    return std::nullopt;
}
