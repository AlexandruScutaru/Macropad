#include "AudioOutputSwitcher.h"
#include "WinApiWrapper.h"

#include <QDebug>

#include <algorithm>


AudioOutputSwitcher::AudioOutputSwitcher(QObject *parent)
    : QObject{parent}
{
    qDebug() << "AudioOutputSwitcher::AudioOutputSwitcher";
    WinApiWrapper::RegisterGlobalShortcut(WinApiWrapper::HotKeys::CYCLE_AUDIO_OUTPUTS);
}

AudioOutputSwitcher::~AudioOutputSwitcher() {
    qDebug() << "AudioOutputSwitcher::~AudioOutputSwitcher";
}


void AudioOutputSwitcher::onHotKeyTriggered() {
    qDebug() << "switch output hoykey pressed";

    auto devices = WinApiWrapper::EnumerateOutputDevices();
    if (devices.endpoints.empty()) {
        qDebug() << "no output devices connected";
        return;
    }

    qDebug() << "Default device: " << devices.defaultDeviceID;
    qDebug() << "Available devices";
    for (const auto& device: devices.endpoints) {
        auto deb = qDebug();
        deb << "index: " << device.index << " decsription: " << device.description << " id: " << device.id;
    }

    auto res = std::find_if(
        devices.endpoints.begin(), devices.endpoints.end(),
        [id = devices.defaultDeviceID](const auto& device) {
            return id == device.id;
        }
        );

    if (res == devices.endpoints.end()) {
        WinApiWrapper::SetDefaultDevice(devices.endpoints[0].id);
        return;
    }

    auto currentIndex = std::distance(devices.endpoints.begin(), res);
    currentIndex = (currentIndex + 1) % devices.endpoints.size();
    WinApiWrapper::SetDefaultDevice(devices.endpoints[currentIndex].id);
}
