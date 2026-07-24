#pragma once

#include <pulse/pulseaudio.h>


struct PulseRequest;

namespace osal {
    class Audio {
    public:
        Audio() = default;
        ~Audio();

        bool initialize();

        bool increaseVolume(int step);
        bool decreaseVolume(int step);
        bool toggleMute();

    private:
        void disconnect();
        bool changeDefaultSinkVolume(int step, bool increase);
        bool getSinkInfo(PulseRequest& request);

        pa_context* mContext{ nullptr };
        pa_mainloop* mMainloop{ nullptr };
        bool mIsConnected{ false };

    };
}
