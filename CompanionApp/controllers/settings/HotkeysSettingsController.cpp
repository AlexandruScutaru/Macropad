#include "controllers/settings/HotkeysSettingsController.h"
#include "controllers/settings/HotkeyActions.h"
#include "os/windows/WinApiWrapper.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

#include <algorithm>


HotkeysSettingsController::HotkeysSettingsController(QObject *parent)
    : QObject(parent)
    , mModel(new HotkeysSettingsModel(this))
{
    qDebug() << "HotkeysSettingsController::HotkeysSettingsController";
    for (auto i = 0; i < static_cast<unsigned int>(Hotkeys::Actions::ACTIONS_COUNT); i++) {
        mCurrentActionsMap[static_cast<Hotkeys::Actions>(i)] = Qt::Key_unknown;
    }
}

HotkeysSettingsController::~HotkeysSettingsController() {
    qDebug() << "HotkeysSettingsController::~HotkeysSettingsController";
}


void HotkeysSettingsController::init(Config* config) {
    mConfig = config;

    for (const auto& [action, key]: mConfig->hotkeyActionMap()) {
        mCurrentActionsMap[action] = key;
    }

    updateModel();
}

HotkeysSettingsModel* HotkeysSettingsController::getModel() {
    return mModel;
}

void HotkeysSettingsController::unregisterHotkeys() {
    WinApiWrapper::ClearGlobalHotkeys();
}

void HotkeysSettingsController::restoreHotkeys() {
    for (const auto& [action, key]: mConfig->hotkeyActionMap()) {
        if (key == Qt::Key_unknown) {
            continue;
        }

        const auto actionVal = static_cast<int>(action);
        WinApiWrapper::RegisterGlobalHotkey(actionVal, key);
    }
}

void HotkeysSettingsController::saveChanges() {
    mConfig->saveHotkeyActionMap(mCurrentActionsMap);

    mHasPendingChanges = false;
    emit hasPendingChangesChanged(mHasPendingChanges);
}

bool HotkeysSettingsController::getHasPendingChanges() {
    return mHasPendingChanges;
}

void HotkeysSettingsController::setActionKeyMapping(int actionVal, int key) {
    const auto savedActionsMap = mConfig->hotkeyActionMap();
    mCurrentActionsMap[static_cast<Hotkeys::Actions>(actionVal)] = key;

    bool areEqual = std::equal(savedActionsMap.begin(), savedActionsMap.end(), mCurrentActionsMap.begin());
    if (mHasPendingChanges == areEqual) {
        mHasPendingChanges = !areEqual;
        emit hasPendingChangesChanged(mHasPendingChanges);
    }

    updateModel();
}

void HotkeysSettingsController::updateModel() {
    mModel->reset();

    std::vector<HotkeysSettingsModel::Row> model;
    model.reserve(mCurrentActionsMap.size());

    for (const auto& [action, key]: mCurrentActionsMap) {
        HotkeysSettingsModel::Row row;
        row[HotkeysSettingsModel::IdRole] = static_cast<int>(action);
        row[HotkeysSettingsModel::NameRole] = Hotkeys::GetActionNameFromEnum(action);
        row[HotkeysSettingsModel::KeyRole] = key;
        model.push_back(row);
    }

    mModel->setData(model);
}
