#pragma once

#include "controller/ActionConfigController.h"
#include "controller/AvailableActionsController.h"
#include "controller/KeypadController.h"
#include "misc/TaskRunner.h"

#include <QObject>
#include <QPointer>
#include <QQmlEngine>

#include <string>
#include <unordered_map>


class AppSettings;
class KeypadService;

class IActionHandler;
using IActionHandlerPtr = std::shared_ptr<IActionHandler>;


class KeypadModule : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Not intended to be created from QML directly")
public:
    explicit KeypadModule(QPointer<AppSettings> appSettings, QObject* parent = nullptr);
    ~KeypadModule();

    void registerHandler(const IActionHandlerPtr& handler);

    Q_INVOKABLE ActionConfigController* getActionConfigController();
    Q_INVOKABLE AvailableActionsController* getAvailableActionsController();
    Q_INVOKABLE KeypadController* getKeypadController();

private slots:
    void onProfileLoaded(const Keypad::Profile& profile);
    void onActionAssignRequested(int layer, int key, const QString& actionId);
    void onKeySelected(int layer, int key);
    void onKeyTriggered(int layer, int key);
    void onConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value);

private:
    QPointer<ActionConfigController> mActionConfigController{ nullptr };
    QPointer<AvailableActionsController> mAvailableActionsController{ nullptr };
    QPointer<KeypadController> mKeypadController{ nullptr };
    QPointer<KeypadService> mService{ nullptr };

    Keypad::Profile mCurrentProfile;
    Keypad::AvailableActions mAvailableActions;
    std::unordered_map<std::string, IActionHandlerPtr> mActionHandlers;

    async::TaskRunner mTaskRunner;

};
