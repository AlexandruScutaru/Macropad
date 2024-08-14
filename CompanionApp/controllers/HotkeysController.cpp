#include "HotkeysController.h"
#include "controllers/HotkeyActions.h"
#include "os/windows/WinApiWrapper.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

#include <algorithm>


HotkeysController::HotkeysController(QObject *parent)
    : QObject(parent)
    , mModel(new HotkeyActionsModel(this))
{
    qDebug() << "HotkeysController::HotkeysController";
    for (auto i = 0; i < static_cast<unsigned int>(Hotkeys::Actions::ACTIONS_COUNT); i++) {
        mActionsMap[static_cast<Hotkeys::Actions>(i)] = Qt::Key_unknown;
    }

    readHotkeyActions();
    updateModel();
}

HotkeysController::~HotkeysController() {
    qDebug() << "HotkeysController::~HotkeysController";
}


HotkeyActionsModel* HotkeysController::getModel() {
    return mModel;
}

void HotkeysController::unregisterHotkeys() {
    mActionsMapBackup = mActionsMap;
    WinApiWrapper::ClearGlobalHotkeys();
}

void HotkeysController::restoreHotkeys() {
    for (const auto& [action, key]: mActionsMapBackup) {
        if (key == Qt::Key_unknown) {
            continue;
        }

        const auto actionVal = static_cast<int>(action);
        WinApiWrapper::RegisterGlobalHotkey(actionVal, key);
    }
}

void HotkeysController::saveChanges() {
    mActionsMapBackup = mActionsMap;
    writeHotkeyActions();

    mHasPendingChanges = false;
    emit hasPendingChangesChanged(mHasPendingChanges);
}

bool HotkeysController::getHasPendingChanges() {
    return mHasPendingChanges;
}

void HotkeysController::setActionKeyMapping(int actionVal, int key) {
    mActionsMap[static_cast<Hotkeys::Actions>(actionVal)] = key;

    bool areEqual = std::equal(mActionsMapBackup.begin(), mActionsMapBackup.end(), mActionsMap.begin());
    if (mHasPendingChanges == areEqual) {
        mHasPendingChanges = !areEqual;
        emit hasPendingChangesChanged(mHasPendingChanges);
    }

    updateModel();
}

void HotkeysController::onHotKeyTriggered(int key) {
    const auto it = std::find_if(mActionsMap.begin(), mActionsMap.end(), [key](const auto& entry) {
        return entry.second == key;
    });

    if (it != mActionsMap.end()) {
        emit hotkeyActionTriggered(it->first);
    }
}

void HotkeysController::updateModel() {
    mModel->reset();

    std::vector<HotkeyActionsModel::Row> model;
    model.reserve(mActionsMap.size());

    for (const auto& [action, key]: mActionsMap) {
        HotkeyActionsModel::Row row;
        row[HotkeyActionsModel::IdRole] = static_cast<int>(action);
        row[HotkeyActionsModel::NameRole] = Hotkeys::GetActionNameFromEnum(action);
        row[HotkeyActionsModel::KeyRole] = key;
        model.push_back(row);
    }

    mModel->setData(model);
}

void HotkeysController::readHotkeyActions() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.beginGroup("hotkeys");

    auto actionsCount = mActionsMap.size();
    auto savedActionsSize = settings.beginReadArray("actions");

    const auto clearGroup = [](auto& settings) {
        settings.remove("");
        settings.endGroup();
    };

    if (actionsCount != static_cast<size_t>(savedActionsSize)) {
        return clearGroup(settings);
    }

    for (int i = 0; i < savedActionsSize; i++) {
        settings.setArrayIndex(i);
        auto action = settings.value("action");
        auto key = settings.value("key");

        if (!action.isValid() || !key.isValid()) {
            return clearGroup(settings);
        }

        const auto actionVal = action.toInt();
        const auto keyVal = key.toInt();

        if (keyVal != Qt::Key_unknown) {
            WinApiWrapper::RegisterGlobalHotkey(actionVal, keyVal);
        }

        mActionsMap[static_cast<Hotkeys::Actions>(actionVal)] = keyVal;
    }

    settings.endArray();
    settings.endGroup();
}

void HotkeysController::writeHotkeyActions() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    settings.beginGroup("hotkeys");
    settings.beginWriteArray("actions");

    // unfortunate naming, the key here is actually a keyboard button :shrug:
    auto index = 0;
    for (const auto& [action, key]: mActionsMap) {
        settings.setArrayIndex(index++);
        const auto actionVal = static_cast<int>(action);
        settings.setValue("action", actionVal);
        settings.setValue("key", key);
    }

    settings.endArray();
    settings.endGroup();
}
