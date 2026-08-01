#include "Utils.h"

#include <ranges>


namespace utils {
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;

        auto tokens = str
            | std::views::split(delimiter)
            | std::views::transform([](auto&& range) {
                return std::string_view{range.begin(), range.end()};
            })
            | std::views::filter([](std::string_view token) {
                return !token.empty();
            });

        for (auto token : tokens) {
            result.emplace_back(token);
        }

        return result;
    }
}
