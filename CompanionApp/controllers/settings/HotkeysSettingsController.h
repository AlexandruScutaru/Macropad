#pragma once

#include "AppSettings.h"
#include "models/settings/HotkeysSettingsModel.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>


class HotkeysSettingsController : public QObject {
    Q_OBJECT
                                                                       // cool naming ig
    Q_PROPERTY(bool hasPendingChanges READ getHasPendingChanges NOTIFY hasPendingChangesChanged)
    QML_ELEMENT
public:
    explicit HotkeysSettingsController(QObject *parent = nullptr);
    ~HotkeysSettingsController();

    Q_INVOKABLE void init(AppSettings* appSettings);
    Q_INVOKABLE HotkeysSettingsModel* getModel();

    Q_INVOKABLE void unregisterHotkeys();
    Q_INVOKABLE void restoreHotkeys();

    Q_INVOKABLE void setActionKeyMapping(int actionVal, int key);
    Q_INVOKABLE void saveChanges();

    bool getHasPendingChanges();

signals:
    void hasPendingChangesChanged(bool hasPendingChanges);

private:
    void updateModel();

    bool mHasPendingChanges{ false };
    QPointer<AppSettings> mAppSettings{ nullptr };
    HotkeyActionMap mCurrentActionsMap;
    QPointer<HotkeysSettingsModel> mModel{ nullptr };

};
