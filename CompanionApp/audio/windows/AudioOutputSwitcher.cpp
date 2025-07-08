#include "../AudioOutputSwitcher.h"

#include "os/windows/WinApiWrapper.h"

#include <QDebug>

#include <algorithm>


AudioOutputSwitcher::AudioOutputSwitcher(QObject* parent)
    : QObject(parent)
{
    qDebug() << "AudioOutputSwitcher::AudioOutputSwitcher";
}

AudioOutputSwitcher::~AudioOutputSwitcher() {
    qDebug() << "AudioOutputSwitcher::~AudioOutputSwitcher";
}


void AudioOutputSwitcher::onSwitchOutputRequested() {
    qDebug() << "AudioOutputSwitcher::onSwitchOutputRequested";

    auto devices = WinApiWrapper::GetOutputDevicesInfo();
    if (devices.endpoints.empty()) {
        qWarning() << "no output devices connected";
        return;
    }

    auto res = std::find_if(
        devices.endpoints.begin(), devices.endpoints.end(),
        [id = devices.defaultDeviceID](const auto& device) {
            return id == device.id;
        }
    );

    if (res == devices.endpoints.end()) {
        WinApiWrapper::SetDefaultOutputDevice(devices.endpoints[0].id);
        return;
    }

    auto index = std::distance(devices.endpoints.begin(), res);
    index = (index + 1) % devices.endpoints.size();
    if (WinApiWrapper::SetDefaultOutputDevice(devices.endpoints[index].id)) {
        qDebug() << "switched audio output to: " << devices.endpoints[index].description;
    }
}
