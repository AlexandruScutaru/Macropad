#pragma once

#include "KeypadTypes.h"
#include "shared/helpers/TaskRunner.h"

#include <nlohmann/json_fwd.hpp>

#include <QObject>
#include <QPointer>

#include <string>
#include <unordered_map>


class Config;

namespace action_handlers {
    class IActionHandler;
    using IActionHandlerPtr = std::shared_ptr<IActionHandler>;
}

class KeypadService: public QObject {
    Q_OBJECT
public:
    explicit KeypadService(::Config* config, QObject* parent = nullptr);
    ~KeypadService();

    void registerHandler(const action_handlers::IActionHandlerPtr& handler);
    void loadSavedProfile();
    void saveProfile();

    Keypad::AvailableActions getAvailableActions() const;
    Keypad::Profile getCurrentProfile() const;

signals:
    void profileChanged(const Keypad::Profile& profile);
    void actionAssigned(int layer, int key, const Keypad::Action& action);
    void availableActionsChanged(const Keypad::AvailableActions& availableActions);
    void actionConfigChanged(int layer, int key, const Keypad::Action& action);
    void actionConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value);

public slots:
    void onProfileLoaded(const Keypad::Profile& profile);
    void onActionAssignRequested(int layer, int key, const QString& actionId);
    void onKeySelected(int layer, int key);
    void onKeyTriggered(int layer, int key);
    void onConfigOptionChanged(int layer, int key, const QString& name, const QVariant& value);

private:
    static QVariant ReadVariant(const nlohmann::json& json, const QString& fieldName, Keypad::OptionType type);
    static void WriteVariant(nlohmann::json& json, const QString& fieldName, Keypad::OptionType type, const QVariant& variant);

    Keypad::Profile mCurrentProfile;
    Keypad::AvailableActions mAvailableActions;
    std::unordered_map<std::string, action_handlers::IActionHandlerPtr> mActionHandlers;

    QPointer<::Config> mConfig{ nullptr };

    async::TaskRunner mTaskRunner;

};
