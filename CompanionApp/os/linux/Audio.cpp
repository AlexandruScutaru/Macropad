#include "Audio.h"

#include <pulse/volume.h>

#include <algorithm>
#include <cstdio>
#include <string>

using namespace osal;


struct PulseRequest {
    bool connected = false;
    bool completed = false;
    bool success = false;
    std::string sinkName;
    bool muteState = false;
    pa_cvolume volume;
};

void context_state_cb(pa_context* context, void* userData) {
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
}

void server_info_cb(pa_context* context, const pa_server_info* info, void* userData) {
    auto* request = static_cast<PulseRequest*>(userData);

    if (!info || !info->default_sink_name) {
        request->success = false;
        request->completed = true;
        return;
    }

    request->sinkName = info->default_sink_name;
    request->success = !request->sinkName.empty();
    request->completed = true;
}

void sink_info_cb(pa_context* context, const pa_sink_info* info, int eol, void* userData) {
    auto* request = static_cast<PulseRequest*>(userData);

    if (eol > 0) {
        request->completed = true;
        return;
    }

    if (!info) {
        request->success = false;
        request->completed = true;
        return;
    }

    request->sinkName = info->name ? info->name : "";
    request->volume = info->volume;
    request->muteState = info->mute != 0;
    request->success = true;
}

void success_cb(pa_context* context, int success, void* userData) {
    auto* request = static_cast<PulseRequest*>(userData);

    request->success = success != 0;
    request->completed = true;
}


Audio::~Audio() {
    disconnect();
}


bool Audio::increaseVolume(int step) {
    return changeDefaultSinkVolume(step, true);
}

bool Audio::decreaseVolume(int step) {
    return changeDefaultSinkVolume(step, false);
}

bool Audio::toggleMute() {
    PulseRequest request;
    if (!getSinkInfo(request)) {
        return false;
    }

    request.completed = false;
    request.success = false;
    pa_context_set_sink_mute_by_name(mContext, request.sinkName.c_str(), !request.muteState, success_cb, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}


bool Audio::initialize() {
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

    PulseRequest request;
    pa_context_set_state_callback(mContext, context_state_cb, &request);

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

bool Audio::changeDefaultSinkVolume(int step, bool increase) {
    PulseRequest request;
    if (!getSinkInfo(request)) {
        return false;
    }

    pa_cvolume newVolume = request.volume;
    pa_volume_t value = (PA_VOLUME_NORM * step) / 100;
    if (increase) {
        pa_cvolume_inc(&newVolume, value);
    } else {
        pa_cvolume_dec(&newVolume, value);
    }

    for (unsigned i = 0; i < newVolume.channels; i++) {
        if (newVolume.values[i] < PA_VOLUME_MUTED) {
            newVolume.values[i] = PA_VOLUME_MUTED;
        } else if (newVolume.values[i] > PA_VOLUME_MAX) {
            newVolume.values[i] = PA_VOLUME_MAX;
        }
    }

    request.completed = false;
    request.success = false;
    pa_context_set_sink_volume_by_name(mContext, request.sinkName.c_str(), &newVolume, success_cb, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}

bool Audio::getSinkInfo(PulseRequest& request) {
    if (!mIsConnected) {
        return false;
    }

    request.completed = false;
    request.success = false;
    pa_context_get_server_info(mContext, server_info_cb, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    if (!request.success || request.sinkName.empty()) {
        return false;
    }

    request.completed = false;
    request.success = false;
    pa_context_get_sink_info_by_name(mContext, request.sinkName.c_str(), sink_info_cb, &request);
    while (!request.completed) {
        pa_mainloop_iterate(mMainloop, 1, nullptr);
    }

    return request.success;
}
