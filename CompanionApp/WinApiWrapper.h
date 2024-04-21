#pragma once

#include <string>
#include <vector>


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
    enum class HotKeys {
        CYCLE_AUDIO_OUTPUTS = 0
    };

    static OutputDevices EnumerateOutputDevices();
    static bool SetDefaultDevice(const std::string& id);
    static bool RegisterGlobalShortcut(HotKeys hotKey);

private:
    WinApiWrapper() {};

    struct HotKeyInfo {
        unsigned int mod = 0;
        unsigned int key = 0;
    };

    static const std::vector<HotKeyInfo> mHotKeys;

};
