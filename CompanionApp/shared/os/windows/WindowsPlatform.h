#pragma once

#include "../IPlatform.h"


namespace os {
    class WindowsPlatform: public IPlatform {
    public:
        WindowsPlatform();

        bool openWebsite(const std::string& address) override;
        bool launch(const std::string& appName, const std::vector<std::string>& args, const std::string& workingDir) override;

        bool incVolume() override;
        bool decVolume() override;
        bool toggleMute() override;
        bool switchOutput() override;

    };
}
