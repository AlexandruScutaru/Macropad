#include "Utils.h"

#include <ranges>


namespace utils {
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        for (const auto& range : str | std::views::split(delimiter)) {
            result.emplace_back(range.begin(), range.end());
        }

        return result;
    }
}
