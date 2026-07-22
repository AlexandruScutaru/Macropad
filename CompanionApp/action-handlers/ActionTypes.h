#pragma once

#include <string>
#include <vector>


namespace action_handlers {
    enum class OptionType {
        String = 0
    };

    struct Config {
        std::string name;
        std::string displayName;
        std::string tooltip;
        OptionType type{ OptionType::String };
    };
    using Configs = std::vector<Config>;

    struct Action {
        std::string id;
        std::string displayName;
        std::string tooltip;
        Configs configs;
    };
    using Actions = std::vector<Action>;

    struct Section {
        std::string id;
        std::string displayName;
        Actions actions;
    };
}
