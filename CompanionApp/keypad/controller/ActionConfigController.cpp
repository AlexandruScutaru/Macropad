#include "ActionConfigController.h"

#include "../KeypadTypes.h"
#include "../model/ActionsListModel.h"

#include <QDebug>


ActionConfigController::ActionConfigController(QObject* parent)
    : QObject(parent)
{
    qDebug() << "ActionConfigController::ActionConfigController";
}

ActionConfigController::~ActionConfigController() {
    qDebug() << "ActionConfigController::~ActionConfigController";
}


void ActionConfigController::optionChanged(const QString& name, const QVariant& value) {
    emit configOptionChanged(mKeyActionLayer, mKeyAction, name, value);
}

QString ActionConfigController::keyActionName() {
    return mActionName;
}

QString ActionConfigController::keyActionDisplayName() {
    return mActionDisplayName;
}

ActionConfigListModel* ActionConfigController::model() {
    return mActionConfigListModel;
}

void ActionConfigController::onActionConfigChanged(int layer, int key, const Keypad::Action& action) {
    if (mActionConfigListModel) {
        mActionConfigListModel->deleteLater();
    }

    QList<QMap<int, QVariant>> configModel;
    for (const auto& config: action.configs) {
        QMap<int, QVariant> configRow;
        configRow[ActionConfigListModel::Type] = config.type;
        configRow[ActionConfigListModel::Name] = config.name;
        configRow[ActionConfigListModel::DisplayName] = config.displayName;
        configRow[ActionConfigListModel::Tooltip] = config.tooltip;
        configRow[ActionConfigListModel::Value] = config.value;
        configModel.push_back(configRow);
    }

    mActionConfigListModel = new ActionConfigListModel(this);
    mActionConfigListModel->setData(configModel);
    emit modelChanged(mActionConfigListModel);
    mActionName = action.name;
    emit keyActionNameChanged(mActionName);
    mActionDisplayName = action.displayName;
    emit keyActionDisplayNameChanged(mActionDisplayName);

    mKeyActionLayer = layer;
    mKeyAction = key;
}
