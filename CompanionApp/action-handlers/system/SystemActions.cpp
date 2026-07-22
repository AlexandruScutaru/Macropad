#include "SystemActions.h"


using namespace action_handlers;

static constexpr std::string HANDLER_ID = "system";


SystemActions::SystemActions() {}


std::string SystemActions::id() {
    return HANDLER_ID;
}

bool SystemActions::handleAction() {
    return true;
};

action_handlers::Section SystemActions::getActions() {
    return {
        .id = HANDLER_ID,
        .displayName = "System",
        .actions = {
            Action {
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
            }
        }
    };
}
