#pragma once

#include <pulse/pulseaudio.h>

#include <mutex>


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
        bool switchOutput();

    private:
        void disconnect();

        bool setDefaultSinkVolume(int step, bool increase);

        bool getServerInfo(PulseRequest& request);
        bool getSinkInfo(PulseRequest& request);
        bool getSinksList(PulseRequest& request);
        bool getSinkInputsList(PulseRequest& request);

        pa_context* mContext{ nullptr };
        pa_mainloop* mMainloop{ nullptr };
        bool mIsConnected{ false };
        std::mutex mMutex;

        static void successCallback(pa_context* context, int success, void* userData);

    };
}
