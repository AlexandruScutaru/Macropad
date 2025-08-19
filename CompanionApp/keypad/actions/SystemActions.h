#pragma once

#include "../service/KeypadService.h"
#include "../KeypadTypes.h"


namespace Keypad::Actions {
    void InsertSystemActions(AvailableActions& actions) {
        const Action openWebsite = { "system_website", KeypadService::tr("Open website"), KeypadService::tr("Opens a website in the default browser application"), "sliders_icon.svg", {
            { "address", KeypadService::tr("Address"), KeypadService::tr("Type the address to open in the browser"), OptionType::String }
        }};

        actions.actionsMap[openWebsite.name] = openWebsite;

        actions.sections.push_back(
            { KeypadService::tr("System"), "sliders_icon.svg", {
                openWebsite.name
            }}
        );
    }
}
