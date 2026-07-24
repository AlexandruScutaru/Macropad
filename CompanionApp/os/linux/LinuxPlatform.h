#pragma once

#include "Audio.h"
#include "../IPlatform.h"


namespace osal {
    class LinuxPlatform: public IPlatform {
    public:
        LinuxPlatform();

        bool openWebsite(const std::string& address) override;

        bool incVolume() override;
        bool decVolume() override;
        bool toggleMute() override;

    private:
        Audio mAudio;

    };
}
