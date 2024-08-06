#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <audiopolicy.h>
#include <windows.h>
#include <mmdeviceapi.h>


template<typename T>
using winapi_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

enum class HotKeys {
    CYCLE_AUDIO_OUTPUTS = 0,

    UKNOWN
};

struct DeviceInfo {
    int index = 0;
    std::string description;
    std::string id;
};

struct OutputDevices {
    std::string defaultDeviceID;
    std::vector<DeviceInfo> endpoints;
};


class WinApiWrapper {
public:
    static OutputDevices GetOutputDevicesInfo();
    static bool SetDefaultOutputDevice(const std::string& id);
    static void SetSessionVolume(const std::string& name, int volume);

    static bool RegisterGlobalShortcut(HotKeys hotKey);
    static HotKeys ParseHotKeyMessageParam(long long lParam);

private:
    WinApiWrapper() {};

    static winapi_unique_ptr<IMMDeviceEnumerator> GetOutputDevicesEnumerator();
    static void IterateOutputDevices(IMMDeviceEnumerator* enumerator, const std::function<void(ULONG, IMMDevice*)>& predicate);
    static void FindAudioSessionByName(const std::string& name, const std::function<void(ISimpleAudioVolume*)>& predicate);
    static std::string GetDefaultOutputDeviceId(IMMDeviceEnumerator* enumerator);
    static std::string GetOutputDeviceName(IMMDevice* device);

    struct HotKeyInfo {
        unsigned int mod = 0;
        unsigned int key = 0;
    };

    static const std::vector<HotKeyInfo> mHotKeys;

};
