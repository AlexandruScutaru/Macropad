#include "Audio.h"

#include <pulse/volume.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace osal;


struct SinkInfo {
    pa_cvolume volume;
    std::string name;
    uint32_t index{ 0U };
    bool muted{ false };
};

struct SinkInputInfo {
    std::string name;
    std::string appName;
    std::string binaryName;

    uint32_t index{ 0U };
    uint32_t sinkIndex{ 0U };
};

struct PulseRequest {
    std::vector<SinkInfo> sinks;
    std::vector<SinkInputInfo> sinkInputs;

    SinkInfo currentSink;

    bool connected = false;
    bool completed = false;
    bool success = false;
};

Audio::~Audio() {
    disconnect();
}

bool Audio::initialize() {
    std::scoped_lock lock(mMutex);

    mMainloop = pa_mainloop_new();
    if (!mMainloop) {
        std::fprintf(stderr, "pa_mainloop_new failed\n");
        return false;
    }

    pa_mainloop_api* api = pa_mainloop_get_api(mMainloop);
    mContext = pa_context_new(api, "macropad");
    if (!mContext) {
        std::fprintf(stderr, "pa_context_new failed\n");
        pa_mainloop_free(mMainloop);
        mMainloop = nullptr;
        return false;
    }

    auto contextStateCallback = [](pa_context* context, void* userData) {
        auto* request = static_cast<PulseRequest*>(userData);

        switch (pa_context_get_state(context)) {
            case PA_CONTEXT_READY:
                request->connected = true;
                request->completed = true;
                break;

            case PA_CONTEXT_FAILED:
            case PA_CONTEXT_TERMINATED:
                request->connected = false;
                request->completed = true;
                break;

            default:
                break;
        }
    };

    PulseRequest request;
    pa_context_set_state_callback(mContext, contextStateCallback, &request);

    if (pa_context_connect(mContext, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr) < 0) {
        std::fprintf(stderr, "pa_context_connect failed\n");
        pa_context_unref(mContext);
        pa_mainloop_free(mMainloop);
        mContext = nullptr;
        mMainloop = nullptr;
        return false;
    }

    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    mIsConnected = request.connected;
    return mIsConnected;
}

void Audio::disconnect() {
    std::scoped_lock lock(mMutex);

    if (mContext) {
        pa_context_disconnect(mContext);
        pa_context_unref(mContext);
        mContext = nullptr;
    }

    if (mMainloop) {
        pa_mainloop_free(mMainloop);
        mMainloop = nullptr;
    }

    mIsConnected = false;
}


bool Audio::increaseVolume(int step) {
    std::scoped_lock lock(mMutex);
    return setDefaultSinkVolume(step, true);
}

bool Audio::decreaseVolume(int step) {
    std::scoped_lock lock(mMutex);
    return setDefaultSinkVolume(step, false);
}

bool Audio::toggleMute() {
    std::scoped_lock lock(mMutex);

    PulseRequest request;
    if (!getSinkInfo(request)) {
        return false;
    }

    request.completed = false;
    request.success = false;
    const auto& sink = request.currentSink;
    pa_context_set_sink_mute_by_name(mContext, sink.name.c_str(), !sink.muted, successCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}

bool Audio::switchOutput() {
    // pactl list short sinks
    // lists all sinks:
    //     sink_index sink_name driver_name specs

    // pactl set-default-sink <sink_name>
    // sets <sink_name> as the default sink (ie default output device)

    // pactl list short sink-inputs
    // lists all sink inputs (aplications providing audio stream to the sink):
    //     sink_input_index sink_index client_index driver_name specs

    // pactl list sink-inputs
    // lists verbose information of sink inputs

    // pactl move-sink-input <sink_input_index> <sink_index>
    // moves an active application denoted by <sink_input_index> to the device output denoted by <sink_index>

    std::scoped_lock lock(mMutex);

    PulseRequest request;
    if (!getSinkInfo(request)) {
        return false;
    }

    std::printf("current sink: index: %d, name: '%s'\n", request.currentSink.index, request.currentSink.name.c_str());

    if (!getSinksList(request)) {
        return false;
    }

    if (request.sinks.size() == 1) {
        return true;
    }

    auto it = std::find_if(
        request.sinks.begin(), request.sinks.end(),
        [name = request.currentSink.name](const auto& sinkInfo) { return sinkInfo.name == name; }
    );

    size_t currentLocalIndex = std::distance(request.sinks.begin(), it);
    if (it == request.sinks.end()) {
        return false;
    }

    auto nextLocalIndex = (currentLocalIndex + 1) % request.sinks.size();
    auto nextSink = request.sinks[nextLocalIndex];

    std::printf("new sink: index: %d name: '%s'\n", nextSink.index, nextSink.name.c_str());

    request.completed = false;
    request.success = false;
    pa_context_set_default_sink(mContext, nextSink.name.c_str(), successCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    if (!request.success) {
        return false;
    }

    if (!getSinkInputsList(request)) {
        return false;
    }

    std::printf("moving sink inputs to sink index: %d\n", nextSink.index);
    for (auto sinkInput : request.sinkInputs) {
        std::printf(
            "\tindex: %d, name: '%s', appName: '%s', binaryName: '%s'\n",
            sinkInput.index, sinkInput.name.c_str(), sinkInput.appName.c_str(), sinkInput.binaryName.c_str()
        );

        request.completed = false;
        request.success = false;
        pa_context_move_sink_input_by_index(mContext, sinkInput.index, nextSink.index, successCallback, &request);
        while (!request.completed) {
            pa_mainloop_iterate(mMainloop, 1, nullptr);
        }
    }

    return request.success;
}


bool Audio::setDefaultSinkVolume(int step, bool increase) {
    PulseRequest request;
    if (!getSinkInfo(request)) {
        return false;
    }

    const auto& sink = request.currentSink;
    pa_cvolume newVolume = sink.volume;

    pa_volume_t value = (PA_VOLUME_NORM * step) / 100;
    if (increase) {
        pa_cvolume_inc(&newVolume, value);
    } else {
        pa_cvolume_dec(&newVolume, value);
    }

    for (auto i = 0; i < newVolume.channels; i++) {
        newVolume.values[i] = std::clamp(newVolume.values[i], PA_VOLUME_MUTED, PA_VOLUME_MAX);
    }

    request.completed = false;
    request.success = false;
    pa_context_set_sink_volume_by_name(mContext, sink.name.c_str(), &newVolume, successCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}

bool Audio::getServerInfo(PulseRequest& request) {
    auto serverInfoCallback = [](pa_context* context, const pa_server_info* info, void* userData) {
        auto* request = static_cast<PulseRequest*>(userData);

        if (!info || !info->default_sink_name) {
            request->success = false;
            request->completed = true;
            return;
        }

        request->currentSink.name = info->default_sink_name;
        request->success = !request->currentSink.name.empty();
        request->completed = true;
    };

    request.completed = false;
    request.success = false;
    pa_context_get_server_info(mContext, serverInfoCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}

bool Audio::getSinkInfo(PulseRequest& request) {
    if (!mIsConnected) {
        return false;
    }

    if (!getServerInfo(request)) {
        return false;
    }

    auto sinkInfoCallback = [](pa_context* context, const pa_sink_info* info, int eol, void* userData) {
        auto* request = static_cast<PulseRequest*>(userData);

        if (eol > 0) {
            request->completed = true;
            return;
        }

        if (!info || !info->name) {
            request->success = false;
            request->completed = true;
            return;
        }

        SinkInfo sink = {
            .volume = info->volume,
            .name = info->name,
            .index = info->index,
            .muted = info->mute != 0
        };

        request->currentSink = sink;
        request->success = !sink.name.empty();
    };

    request.completed = false;
    request.success = false;
    pa_context_get_sink_info_by_name(mContext, request.currentSink.name.c_str(), sinkInfoCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}

bool Audio::getSinksList(PulseRequest& request) {
    auto sinkListCallback = [](pa_context* context, const pa_sink_info* info, int eol, void* userData) {
        auto* request = static_cast<PulseRequest*>(userData);

        if (eol > 0) {
            request->completed = true;
            return;
        }

        if (!info || !info->name) {
            request->success = false;
            request->completed = true;
            return;
        }

        request->sinks.push_back(SinkInfo{
            .name = info->name,
            .index = info->index
        });

        request->success = true;
    };

    request.completed = false;
    request.success = false;
    request.sinks.clear();
    request.sinkInputs.clear();
    pa_context_get_sink_info_list(mContext, sinkListCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success && !request.sinks.empty();
}

bool Audio::getSinkInputsList(PulseRequest& request) {
    auto sinkInputListCallback = [](pa_context* context, const pa_sink_input_info* info, int eol, void* userData) {
        auto* request = static_cast<PulseRequest*>(userData);

        if (eol > 0) {
            request->completed = true;
            return;
        }

        if (!info || !info->name) {
            return;
        }

        SinkInputInfo sinkInput;
        sinkInput.name = info->name;
        sinkInput.index = info->index;
        sinkInput.sinkIndex = info->sink;

        if (info->proplist) {
            sinkInput.appName = pa_proplist_gets(info->proplist, PA_PROP_APPLICATION_NAME);
            sinkInput.binaryName = pa_proplist_gets(info->proplist, PA_PROP_APPLICATION_PROCESS_BINARY);
        }

        request->sinkInputs.push_back(sinkInput);
        request->success = true;
    };

    request.completed = false;
    request.success = false;
    request.sinkInputs.clear();
    pa_context_get_sink_input_info_list(mContext, sinkInputListCallback, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}


void Audio::successCallback(pa_context* context, int success, void* userData) {
    auto* request = static_cast<PulseRequest*>(userData);

    request->success = success != 0;
    request->completed = true;
}
