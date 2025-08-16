#pragma once

#include "../KeypadModule.h"
#include "../KeypadTypes.h"


namespace Keypad::Actions {
    void InsertSystemActions(ActionsMap& actionsMap, Sections& sections) {
        const Action openWebsite = { KeypadModule::Id("system", "website"), KeypadModule::tr("Open website"), KeypadModule::tr("Opens a website in the default browser application"), "sliders_icon.svg", {
            { KeypadModule::tr("Address"), KeypadModule::tr("Type the address to open in the browser"), OptionType::String }
        }};

        actionsMap[openWebsite.id] = openWebsite;

        sections.push_back(
            { KeypadModule::tr("System"), "sliders_icon.svg", {
                openWebsite.id
            }}
        );
    }
}
