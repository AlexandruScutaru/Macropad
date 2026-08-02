#include "ActionConfigController.h"

#include "../KeypadTypes.h"
#include "../model/ActionsListModel.h"

#include <QDebug>


ActionConfigController::ActionConfigController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "ActionConfigController::ActionConfigController";

    mActionConfigListModel = new ActionConfigListModel(this);
}

ActionConfigController::~ActionConfigController() {
    qDebug() << "ActionConfigController::~ActionConfigController";
}


void ActionConfigController::optionChanged(const QString& name, const QVariant& value) {
    emit configOptionChanged(mKeyActionLayer, mKeyAction, name, value);
}

QString ActionConfigController::keyActionId() {
    return mActionId;
}

QString ActionConfigController::keyActionDisplayName() {
    return mActionDisplayName;
}

ActionConfigListModel* ActionConfigController::model() {
    return mActionConfigListModel;
}

void ActionConfigController::onActionConfigChanged(int layer, int key, const Keypad::Action& action) {
    if (!mActionConfigListModel) {
        mActionConfigListModel = new ActionConfigListModel(this);
    }

    mActionConfigListModel->reset();

    QList<QMap<int, QVariant>> configModel;
    configModel.reserve(action.configs.size());

    for (const auto& config: action.configs) {
        QMap<int, QVariant> configRow;
        configRow[ActionConfigListModel::Type] = config.type;
        configRow[ActionConfigListModel::Name] = config.name;
        configRow[ActionConfigListModel::DisplayName] = config.displayName;
        configRow[ActionConfigListModel::Tooltip] = config.tooltip;
        configRow[ActionConfigListModel::Value] = config.value;
        configRow[ActionConfigListModel::WantFolder] = config.wantFolder;

        configModel.push_back(configRow);
    }

    mActionConfigListModel->setData(configModel);
    emit modelChanged(mActionConfigListModel);
    mActionId = action.id;
    emit keyActionIdChanged(mActionId);
    mActionDisplayName = action.displayName;
    emit keyActionDisplayNameChanged(mActionDisplayName);

    mKeyActionLayer = layer;
    mKeyAction = key;
}

void ActionConfigController::onActionConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value) {
    if (!mActionConfigListModel) {
        return;
    }

    if (layer != mKeyActionLayer || key != mKeyAction) {
        return;
    }

    QModelIndex modelIndex;
    size_t i = 0;
    for (; i < mActionConfigListModel->getCount(); i++) {
        auto index = mActionConfigListModel->index(i, 0);
        if (mActionConfigListModel->data(index, ActionConfigListModel::Name).toString() == name) {
            modelIndex = index;
            break;
        }
    }

    if (!modelIndex.isValid()) {
        return;
    }

    QMap<int, QVariant> row;
    row[ActionConfigListModel::Type] = mActionConfigListModel->data(modelIndex, ActionConfigListModel::Type);
    row[ActionConfigListModel::Name] = name;
    row[ActionConfigListModel::DisplayName] = mActionConfigListModel->data(modelIndex, ActionConfigListModel::DisplayName);
    row[ActionConfigListModel::Tooltip] = mActionConfigListModel->data(modelIndex, ActionConfigListModel::Tooltip);
    row[ActionConfigListModel::Value] = value;
    row[ActionConfigListModel::WantFolder] = mActionConfigListModel->data(modelIndex, ActionConfigListModel::WantFolder);

    mActionConfigListModel->updateRow(i, row);
}
