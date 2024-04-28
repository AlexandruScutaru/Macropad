#pragma once

#include <string>
#include <vector>


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
    static OutputDevices EnumerateOutputDevices();
    static bool SetDefaultDevice(const std::string& id);
    static bool RegisterGlobalShortcut(HotKeys hotKey);
    static HotKeys ParseHotKeyMessageParam(long long lParam);

private:
    WinApiWrapper() {};

    struct HotKeyInfo {
        unsigned int mod = 0;
        unsigned int key = 0;
    };

    static const std::vector<HotKeyInfo> mHotKeys;

};
