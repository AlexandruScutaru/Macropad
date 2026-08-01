#pragma once

#include "Audio.h"
#include "../IPlatform.h"


namespace osal {
    class LinuxPlatform: public IPlatform {
    public:
        LinuxPlatform();

        bool openWebsite(const std::string& address) override;
        bool launch(const std::string& appName, const std::vector<std::string>& args, const std::string& workingDir) override;

        bool incVolume() override;
        bool decVolume() override;
        bool toggleMute() override;
        bool switchOutput() override;

    private:
        Audio mAudio;

    };
}
