#pragma once

#include "../IActionHandler.h"

#include <nlohmann/json_fwd.hpp>

#include <string>
#include <unordered_map>


namespace os {
    class IPlatform;
    using IPlatformPtr = std::shared_ptr<IPlatform>;
}

namespace action_handlers {
    class SystemActions: public IActionHandler {
    public:
        SystemActions(os::IPlatformPtr platform);

        std::string id() override;
        action_handlers::Section getActions() override;
        bool handleAction(const std::string& payload) override;

    private:
        bool openWebsite(const nlohmann::json& payload);
        bool launch(const nlohmann::json& payload);

        bool increaseVolume(const nlohmann::json& payload);
        bool decreaseVolume(const nlohmann::json& payload);
        bool toggleMute(const nlohmann::json& payload);
        bool switchOutput(const nlohmann::json& payload);

        std::unordered_map<std::string, bool (SystemActions::*)(const nlohmann::json&)> mActionHandlersMap;
        os::IPlatformPtr mPlatform{ nullptr };

    };
}
