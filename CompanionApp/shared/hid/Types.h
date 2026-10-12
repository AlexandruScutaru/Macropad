#pragma once

#include <cstdint>
#include <functional>
#include <vector>


namespace hid {
    enum class MessageType: uint8_t {
        KeyPress           = 1 << 0,
        EncoderRotation    = 2 << 0,
        SlidersModuleState = 3 << 0,
        SlidersValueChange = 4 << 0,
    };

    using Message = std::vector<unsigned char>;
    using MessageHandlerCb = std::function<void(const Message&)>;

    // maybe move to a newer standard std::expected
    struct ReadResult {
        Message message;
        bool error{ false };
    };
}
