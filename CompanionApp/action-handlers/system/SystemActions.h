#pragma once

#include "../IActionHandler.h"


class SystemActions: public IActionHandler {
public:
    SystemActions();

    std::string id() override;
    action_handlers::Section getActions() override;
    bool handleAction() override;

private:

};
