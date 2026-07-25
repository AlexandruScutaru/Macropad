#pragma once

#include "../IPlatform.h"


namespace osal {
    class WindowsPlatform: public IPlatform {
    public:
        WindowsPlatform();

        bool openWebsite(const std::string& address) override;

        bool incVolume() override;
        bool decVolume() override;
        bool toggleMute() override;

    };
}
