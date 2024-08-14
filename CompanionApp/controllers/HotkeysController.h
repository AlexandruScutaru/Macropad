#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include "models/HotkeyActionsModel.h"

#include <unordered_map>

namespace Hotkeys {
    enum class Actions;
}

using HotkeyActionMap = std::unordered_map<Hotkeys::Actions, int>;


class HotkeysController : public QObject {
    Q_OBJECT
    // cool naming ig
    Q_PROPERTY(bool hasPendingChanges READ getHasPendingChanges NOTIFY hasPendingChangesChanged)
    QML_ELEMENT
public:
    explicit HotkeysController(QObject *parent = nullptr);
    ~HotkeysController();

    Q_INVOKABLE HotkeyActionsModel* getModel();

    Q_INVOKABLE void unregisterHotkeys();
    Q_INVOKABLE void restoreHotkeys();

    Q_INVOKABLE void setActionKeyMapping(int actionVal, int key);
    Q_INVOKABLE void saveChanges();

    bool getHasPendingChanges();

signals:
    void hasPendingChangesChanged(bool hasPendingChanges);
    void hotkeyActionTriggered(Hotkeys::Actions action);

public slots:
    void onHotKeyTriggered(int key);

private:
    void updateModel();
    void readHotkeyActions();
    void writeHotkeyActions();

    bool mHasPendingChanges{ false };

    QPointer<HotkeyActionsModel> mModel{ nullptr };

    HotkeyActionMap mActionsMap;
    HotkeyActionMap mActionsMapBackup;

};
