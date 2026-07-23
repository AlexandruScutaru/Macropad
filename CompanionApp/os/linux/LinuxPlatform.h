#pragma once

#include "../IPlatform.h"


namespace osal {
    class LinuxPlatform: public IPlatform {
    public:
        bool openWebsite(const std::string& address) override;

    };
}
