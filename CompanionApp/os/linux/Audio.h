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

    private:
        void disconnect();
        bool changeDefaultSinkVolume(int step, bool increase);
        bool getSinkInfo(PulseRequest& request);

        pa_context* mContext{ nullptr };
        pa_mainloop* mMainloop{ nullptr };
        bool mIsConnected{ false };
        std::mutex mMutex;

        static void contextStateCallback(pa_context* context, void* userData);
        static void serverInfoCallback(pa_context* context, const pa_server_info* info, void* userData);
        static void sinkInfoCallback(pa_context* context, const pa_sink_info* info, int eol, void* userData);
        static void successCallback(pa_context* context, int success, void* userData);
    };
}
