#include "SystemActions.h"
#include <os/IPlatform.h>

#include <nlohmann/json.hpp>

#include <iostream>


using namespace action_handlers;

static constexpr std::string HANDLER_ID = "system";


SystemActions::SystemActions(osal::IPlatformPtr platform)
    : mPlatform(platform)
{
    assert(mPlatform && "NULL IPlatformPtr instance");
}


std::string SystemActions::id() {
    return HANDLER_ID;
}

bool SystemActions::handleAction(const std::string& payload) {
    std::printf("action handling for: %s\n", payload.c_str());

    const auto& jsonPayload = nlohmann::json::parse(payload, nullptr, false);
    if (jsonPayload.is_discarded()) {
        std::printf("could not parse action handler payload\n");
        return false;
    }

    const auto& actionId = jsonPayload.value<std::string>("id", "");

    if (const auto& it = mActionHandlersMap.find(actionId); it != mActionHandlersMap.end()) {
        if (!it->second) {
            std::printf("no function to handle the action\n");
            return false;
        }

        return ((*this).*it->second)(jsonPayload);
    }

    std::printf("no handler for the action\n");
    return false;
};

bool SystemActions::openWebsite(const nlohmann::json& payload) {
    const auto& address = payload.value<std::string>("address", "");
    if (address.empty()) {
        std::printf("openWebsite: empty address in payload\n");
        return false;
    }

    return mPlatform->openWebsite(address);
}

bool SystemActions::increaseVolume(const nlohmann::json& payload) {
    return mPlatform->incVolume();
}

bool SystemActions::decreaseVolume(const nlohmann::json& payload) {
    return mPlatform->decVolume();
}

bool SystemActions::toggleMute(const nlohmann::json& payload) {
    return mPlatform->toggleMute();
}

bool SystemActions::switchOutput(const nlohmann::json& payload) {
    return mPlatform->switchOutput();
}


action_handlers::Section SystemActions::getActions() {
    const auto openWebsite = Action {
        .id = "website",
        .displayName = "Open website",
        .tooltip = "Opens a website in the default browser application",
        .configs = {
            Config {
                .name = "address",
                .displayName = "Address",
                .tooltip = "The address to open in the browser",
                .type = OptionType::String
            }
        }
    };
    mActionHandlersMap[openWebsite.id] = &SystemActions::openWebsite;

    const auto increaseVolume = Action {
        .id = "volume-up",
        .displayName = "Increase Volume",
        .tooltip = "Increases the volume of the current output device",
    };
    mActionHandlersMap[increaseVolume.id] = &SystemActions::increaseVolume;

    const auto decreaseVolume = Action {
        .id = "volume-down",
        .displayName = "Decrease Volume",
        .tooltip = "Decreases the volume of the current output device",
    };
    mActionHandlersMap[decreaseVolume.id] = &SystemActions::decreaseVolume;

    const auto toggleMute = Action {
        .id = "mute",
        .displayName = "Mute/Unmute",
        .tooltip = "Mutes/unmutes the current output device",
    };
    mActionHandlersMap[toggleMute.id] = &SystemActions::toggleMute;

    const auto switchOutput = Action {
        .id = "switch-output",
        .displayName = "Switch audio output",
        .tooltip = "Cycle through audio devices",
    };
    mActionHandlersMap[switchOutput.id] = &SystemActions::switchOutput;

    return {
        .id = HANDLER_ID,
        .displayName = "System",
        .actions = {
            openWebsite,
            increaseVolume,
            decreaseVolume,
            toggleMute,
            switchOutput
        }
    };
}
