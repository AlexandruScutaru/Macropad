#pragma once

#include "KeypadTypes.h"
#include "ActionTypes.h"

#include <memory>
#include <string>


class IActionHandler {
public:
    virtual ~IActionHandler() = default;

    virtual std::string id() = 0;
    virtual action_handlers::Section getActions() = 0;
    virtual bool handleAction(const std::string& payload) = 0;

};

using IActionHandlerPtr = std::shared_ptr<IActionHandler>;
