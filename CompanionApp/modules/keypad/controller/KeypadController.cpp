#include "KeypadController.h"
#include "../service/KeypadService.h"

#include <QDebug>
#include <qobject.h>


KeypadController::KeypadController(QObject* parent)
    : QObject(parent)
    , mCurrentModel(new KeysListModel(this))
{
    qDebug() << "KeypadController::KeypadController";
}

KeypadController::~KeypadController() {
    qDebug() << "KeypadController::~KeypadController";
}


int KeypadController::layerCount() {
    return mLayerCount;
}

int KeypadController::currentLayer() {
    return mCurrentLayer;
}

QString KeypadController::layerColor() {
    return mLayerColor;
}

KeysListModel* KeypadController::model() {
    return mCurrentModel;
}

void KeypadController::setCurrentLayer(int layer) {
    if (!isIndexInBounds(layer) || mCurrentLayer == layer) {
        return;
    }

    handleLayerChange(layer);
}

void KeypadController::setLayerColor(const QString& color) {
    if (mLayerColor == color) {
        return;
    }

    mLayerColor = color;
    emit layerColorChanged(mLayerColor);
}

void KeypadController::setAppContext(IAppContext* appContext) {
    mAppContext = appContext;
    mKeypadService = mAppContext->keypadService();

    QObject::connect(mKeypadService, &KeypadService::profileChanged, this, &KeypadController::onProfileChanged);
    QObject::connect(mKeypadService, &KeypadService::actionAssigned, this, &KeypadController::onActionAssigned);

    onProfileChanged(mKeypadService->getCurrentProfile());
}

void KeypadController::assignAction(int key, const QString& actionId) {
    if (!isIndexInBounds(mCurrentLayer)) {
        return;
    }

    mKeypadService->onActionAssignRequested(mCurrentLayer, key, actionId);
}

void KeypadController::onKeySelected(int key) {
    mKeypadService->onKeySelected(mCurrentLayer, key);
}

void KeypadController::onKeyTriggered(int key) {
    mKeypadService->onKeyTriggered(mCurrentLayer, key);
}

void KeypadController::onProfileChanged(const Keypad::Profile& profile) {
    for (auto& [_, model]: mLayers) {
        if (model) {
            model->deleteLater();
        }
    }
    mLayers.clear();

    for (const auto& layer: profile.layers) {
        mLayers.emplace_back(layer.color, createKeysListModels(layer));
    }

    auto count = mLayers.size();
    if (count != mLayerCount) {
        mLayerCount = count;
        emit layerCountChanged(mLayerCount);
    }

    if (mLayerCount > 0) {
        handleLayerChange(0);
    } else {
        mCurrentModel = nullptr;
        mLayerColor = "transparent";
        mCurrentLayer = 0;
        emit modelChanged(mCurrentModel);
        emit layerColorChanged(mLayerColor);
        emit currentLayerChanged(mCurrentLayer);
    }
}

void KeypadController::onActionAssigned(int layer, int key, const Keypad::Action& action) {
    QMap<int, QVariant> keyRow;
    keyRow[KeysListModel::Round] = key == 0;
    keyRow[KeysListModel::ActionId] = action.id;
    keyRow[KeysListModel::ActionDisplayName] = action.displayName;
    keyRow[KeysListModel::ActionIcon] = action.iconName;

    auto& [_, model] = mLayers[mCurrentLayer];
    if (model) {
        model->updateRow(key, keyRow);
    }
}

KeysListModel* KeypadController::createKeysListModels(const Keypad::Layer& layer) {
    QList<QMap<int, QVariant>> keysModel;
    for (int key = 0; key < layer.actions.size(); key++) {
        const auto& action = layer.actions[key];
        QMap<int, QVariant> keyRow;
        keyRow[KeysListModel::Round] = key == 0;
        keyRow[KeysListModel::ActionId] = action.id;
        keyRow[KeysListModel::ActionDisplayName] = action.displayName;
        keyRow[KeysListModel::ActionIcon] = action.iconName;
        keysModel.push_back(keyRow);
    }
    const auto keysListModel = new KeysListModel(this);
    keysListModel->setData(keysModel);
    
    return keysListModel;
}

bool KeypadController::isIndexInBounds(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= mLayers.size()) {
        qDebug() << "Index '" << index << "' out of bounds!";
        return false;
    }

    return true;
}

void KeypadController::handleLayerChange(int newLayer) {
    mCurrentLayer = newLayer;
    emit currentLayerChanged(mCurrentLayer);

    const auto& [color, model] = mLayers[mCurrentLayer];
    mLayerColor = color;
    mCurrentModel = model;
    emit layerColorChanged(mLayerColor);
    emit modelChanged(mCurrentModel);
}
