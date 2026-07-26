#pragma once

#include "../IActionHandler.h"

#include <nlohmann/json_fwd.hpp>

#include <string>
#include <unordered_map>


namespace osal {
    class IPlatform;
    using IPlatformPtr = std::shared_ptr<IPlatform>;
}

class SystemActions: public IActionHandler {
public:
    SystemActions(osal::IPlatformPtr platform);

    std::string id() override;
    action_handlers::Section getActions() override;
    bool handleAction(const std::string& payload) override;

private:
    bool openWebsite(const nlohmann::json& payload);

    bool increaseVolume(const nlohmann::json& payload);
    bool decreaseVolume(const nlohmann::json& payload);
    bool toggleMute(const nlohmann::json& payload);
    bool switchOutput(const nlohmann::json& payload);

    std::unordered_map<std::string, bool (SystemActions::*)(const nlohmann::json&)> mActionHandlersMap;
    osal::IPlatformPtr mPlatform{ nullptr };

};
