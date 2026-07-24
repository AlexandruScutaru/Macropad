#pragma once

#include <string>
#include <memory>


namespace osal {
    class IPlatform {
    public:
        ~IPlatform() = default;

        virtual bool openWebsite(const std::string& address) = 0;

        virtual bool incVolume() = 0;
        virtual bool decVolume() = 0;
        virtual bool toggleMute() = 0;

    };

    using IPlatformPtr = std::shared_ptr<IPlatform>;

    // factory method whose implementation is added via corresponding platform file at compile time
    IPlatformPtr CreatePlatform();
}
