#include "WinApiWrapper.h"
#include "IPolicyConfig.h"

#include <audioclient.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlstr.h>
#include <psapi.h>

#include <QDebug>

#include <algorithm>
#include <cctype>


template<typename T>
static void safeRelease(T* ptr) {
    if (ptr) {
        ptr->Release();
    }
}

OutputDevices WinApiWrapper::GetOutputDevicesInfo() {
    qDebug() << "WinApiWrapper::EnumerateOutputDevices";

    OutputDevices devices;

    auto enumerator = GetOutputDevicesEnumerator();
    devices.defaultDeviceID = GetDefaultOutputDeviceId(enumerator.get());

    IterateOutputDevices(enumerator.get(), [&devices](const auto index, const auto device) {
        HRESULT hr = S_OK;

        DWORD state;
        if (hr = device->GetState(&state); FAILED(hr)) {
            qWarning() << "couldn't get device state for index " << index;
            return;
        }

        if (state != DEVICE_STATE_ACTIVE) {
            qWarning() << "device not active for index " << index;
            return;
        }

        LPWSTR pwszID;
        if (hr = device->GetId(&pwszID); FAILED(hr)) {
            qWarning() << "couldn't get device id for index " << index;
            return;
        }
        std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszIDptr(pwszID, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });

        devices.endpoints.emplace_back(index, GetOutputDeviceName(device), std::string(CW2A(pwszID)));
    });

    return devices;
}

bool WinApiWrapper::SetDefaultOutputDevice(const std::string& id) {
    qDebug() << "WinApiWrapper::SetDefaultOutputDevice";

    HRESULT hr = S_OK;

    IPolicyConfig* pPolicyConfig = NULL;
    if (hr = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_ALL, __uuidof(IPolicyConfig), (LPVOID *)&pPolicyConfig); FAILED(hr)) {
        qWarning() << "CoCreateInstance: failed to create IPolicyConfig";
        return false;
    }
    winapi_unique_ptr<IPolicyConfig> enumeratorPtr(pPolicyConfig, safeRelease<IPolicyConfig>);

    if (hr = pPolicyConfig->SetDefaultEndpoint(CA2W (id.c_str()), eMultimedia); FAILED(hr)) {
        qWarning() << "SetDefaultEndpoint failed";
        return false;
    }

    return true;
}

void WinApiWrapper::SetSessionVolume(const std::string& name, int volume) {
    qDebug() << "WinApiWrapper::SetSessionVolume";

    FindAudioSessionByName(name, [name, volume](const auto simpleAudioVolume) {
        if(auto hr = simpleAudioVolume->SetMasterVolume(volume / 100.0f, NULL); FAILED(hr)) {
            qWarning() << "cannot set volume for " << name;
        }
    });
}

winapi_unique_ptr<IMMDeviceEnumerator> WinApiWrapper::GetOutputDevicesEnumerator() {
    qDebug() << "WinApiWrapper::GetOutputDevicesEnumerator";

    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

    IMMDeviceEnumerator* pEnumerator = nullptr;
    if (auto hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator); FAILED(hr)) {
        qWarning() << "CoCreateInstance: failed to create IID_IMMDeviceEnumerator";
        return {};
    }

    return winapi_unique_ptr<IMMDeviceEnumerator>(pEnumerator, safeRelease<IMMDeviceEnumerator>);
}

void WinApiWrapper::IterateOutputDevices(IMMDeviceEnumerator* enumerator, const std::function<void(ULONG, IMMDevice*)>& predicate) {
    qDebug() << "WinApiWrapper::IterateOutputDevices";

    HRESULT hr = S_OK;

    IMMDeviceCollection* pCollection = nullptr;
    if (hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection); FAILED(hr)) {
        qWarning() << "EnumAudioEndpoints: failed";
        return;
    }
    winapi_unique_ptr<IMMDeviceCollection> collectionPtr(pCollection, safeRelease<IMMDeviceCollection>);

    UINT count;
    if(hr = collectionPtr->GetCount(&count); FAILED(hr)) {
        qWarning() << "couldn't get collection device count";
        return;
    }
    qDebug() << "found " << count << " devices";

    for (ULONG i = 0; i < count; i++) {
        IMMDevice* pEndpoint = nullptr;
        if (hr = collectionPtr->Item(i, &pEndpoint); FAILED(hr)) {
            qWarning() << "couldn't get device from collection for index " << i;
            continue;
        }
        winapi_unique_ptr<IMMDevice> endPointPtr(pEndpoint, safeRelease<IMMDevice>);

        predicate(i, endPointPtr.get());
    }
}

void WinApiWrapper::FindAudioSessionByName(const std::string& name, const std::function<void(ISimpleAudioVolume*)>& predicate) {
    qDebug() << "WinApiWrapper::FindAudioSessionByName";

    auto enumerator = GetOutputDevicesEnumerator();

    IterateOutputDevices(enumerator.get(), [name, pred = std::move(predicate)](const auto index, const auto device) {
        qDebug() << "endpoint index " << index;
        qDebug() << "device name " << GetOutputDeviceName(device);

        HRESULT hr = S_OK;

        IAudioSessionManager* pAudioSessionManager = nullptr;
        if (hr = device->Activate(__uuidof(IAudioSessionManager), CLSCTX_ALL, NULL, (void**)&pAudioSessionManager); FAILED(hr)) {
            qWarning() << "could not get IAudioSessionManager for device index " << index;
            return;
        }
        winapi_unique_ptr<IAudioSessionManager> audioSessionManagerPtr(pAudioSessionManager, safeRelease<IAudioSessionManager>);

        IAudioSessionManager2* pAudioSessionManager2 = nullptr;
        if (hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pAudioSessionManager2); FAILED(hr)) {
            qWarning() << "could not get IAudioSessionManager2 for device index " << index;
            return;
        }
        winapi_unique_ptr<IAudioSessionManager2> audioSessionManager2Ptr(pAudioSessionManager2, safeRelease<IAudioSessionManager2>);

        IAudioSessionEnumerator* pAudioSessionEnumerator = nullptr;
        if (hr = audioSessionManager2Ptr->GetSessionEnumerator(&pAudioSessionEnumerator); FAILED(hr)) {
            qWarning() << "could not get IAudioSessionEnumerator for device index " << index;
            return;
        }
        winapi_unique_ptr<IAudioSessionEnumerator> audioSessionEnumeratorPtr(pAudioSessionEnumerator, safeRelease<IAudioSessionEnumerator>);

        int sessionsCount = 0;
        if (hr = audioSessionEnumeratorPtr->GetCount(&sessionsCount); FAILED(hr)) {
            qWarning() << "could not get sessions count for device index " << index;
            return;
        }

        for (int sessionIndex = 0; sessionIndex < sessionsCount; sessionIndex++) {
            IAudioSessionControl* pAudioSessionControl = nullptr;
            if (hr = audioSessionEnumeratorPtr->GetSession(sessionIndex, &pAudioSessionControl); FAILED(hr)) {
                qWarning() << "    IAudioSessionControl: could not get session index " << sessionIndex;
                continue;
            }
            winapi_unique_ptr<IAudioSessionControl> audioSessionControlPtr(pAudioSessionControl, safeRelease<IAudioSessionControl>);

            IAudioSessionControl2* pAudioSessionControl2 = nullptr;
            if (hr = audioSessionControlPtr->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&pAudioSessionControl2); FAILED(hr)) {
                qWarning() << "    IAudioSessionControl: could not get IAudioSessionControl2 reference for session index " << sessionIndex;
                continue;
            }
            winapi_unique_ptr<IAudioSessionControl2> audioSessionControl2Ptr(pAudioSessionControl2, safeRelease<IAudioSessionControl2>);

            if (audioSessionControl2Ptr->IsSystemSoundsSession() == S_OK) {
                qDebug() << "    IAudioSessionControl2: session is System Sounds";
                continue;
            }

            const auto getSessionProcessName = [&audioSessionControl2Ptr, sessionIndex]() -> std::pair<bool, std::string> {
                HRESULT hr = S_OK;

                DWORD processId = 0;
                if (hr = audioSessionControl2Ptr->GetProcessId(&processId); FAILED(hr)) {
                    qWarning() << "    IAudioSessionControl2: could not get processId for session index " << sessionIndex;
                    return { false, "" };
                }

                HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
                if (!processHandle) {
                    qWarning() << "    IAudioSessionControl2: could not get process handle for session index " << sessionIndex;
                    return { false, "" };
                }
                std::unique_ptr<void, void(*)(HANDLE)> processHandlePtr(processHandle, [](HANDLE ptr) { ::CloseHandle(ptr); });

                TCHAR path[MAX_PATH];
                if (!GetModuleFileNameEx(processHandle, 0, path, MAX_PATH))
                {
                    qWarning() << "    IAudioSessionControl2: could not get module file name for session index " << sessionIndex;
                    return { false, "" };
                }

                return { true, std::string(CW2A(path)) };
            };

            LPWSTR pwszSessionName = nullptr;
            std::string sessionName;

            hr = audioSessionControlPtr->GetDisplayName(&pwszSessionName);
            if (FAILED(hr) || pwszSessionName == nullptr || pwszSessionName[0] == 0) {
                const auto result = getSessionProcessName();
                if (!result.first || result.second.empty()) {
                    qWarning() << "    could not get session name for index " << sessionIndex;
                    continue;
                }

                sessionName = result.second;
            } else {
                std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszSessionNamePtr(pwszSessionName, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });
                sessionName = std::string(CW2A(pwszSessionName));
            }

            qDebug() << "    session name " << sessionName;

            const auto toUpper = [](const std::string& str) -> std::string {
                std::string upper;
                std::transform(str.begin(), str.end(), std::back_inserter(upper), toupper);
                return upper;
            };

            if (toUpper(sessionName).find(toUpper(name)) != std::string::npos) {
                /*
                LPWSTR pwszSessionID = nullptr;
                if (hr = audioSessionControl2Ptr->GetSessionInstanceIdentifier(&pwszSessionID); FAILED(hr)) {
                    qWarning() << "    IAudioSessionControl2: couldn't get SessionInstanceIdentifier for session index " << sessionIndex;
                    continue;
                }
                std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszSessionIDPtr(pwszSessionID, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });

                ISimpleAudioVolume* pSimpleAudioVolume = nullptr;
                if (hr = audioSessionManagerPtr->GetSimpleAudioVolume(0, 0, &pSimpleAudioVolume); FAILED(hr)) {
                    qWarning() << "    IAudioSessionManager: could not get ISimpleAudioVolume reference for session index " << sessionIndex;
                    continue;
                }
                winapi_unique_ptr<ISimpleAudioVolume> simpleAudioVolumePtr(pSimpleAudioVolume, safeRelease<ISimpleAudioVolume>);
                */

                ISimpleAudioVolume* pSimpleAudioVolume = nullptr;
                if (hr = audioSessionControlPtr->QueryInterface(IID_PPV_ARGS(&pSimpleAudioVolume)); FAILED(hr)) {
                    qWarning() << "    IAudioSessionControl: could not get ISimpleAudioVolume reference for session index " << sessionIndex;
                    continue;
                }
                winapi_unique_ptr<ISimpleAudioVolume> simpleAudioVolumePtr(pSimpleAudioVolume, safeRelease<ISimpleAudioVolume>);


                pred(simpleAudioVolumePtr.get());
            }
        }
    });
}

std::string WinApiWrapper::GetDefaultOutputDeviceId(IMMDeviceEnumerator* enumerator) {
    qDebug() << "WinApiWrapper::GetDefaultOutputDeviceId";

    HRESULT hr = S_OK;

    IMMDevice* pDefaultEndpoint = nullptr;
    if (hr = enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultEndpoint); FAILED(hr)) {
        qWarning() << "GetDefaultAudioEndpoint: failed";
        return "";
    }
    winapi_unique_ptr<IMMDevice> defaultEndpointPtr(pDefaultEndpoint, safeRelease<IMMDevice>);

    LPWSTR pwszDefaultID;
    if (hr = pDefaultEndpoint->GetId(&pwszDefaultID); FAILED(hr)) {
        qWarning() << "couldn't get default device id";
        return "";
    }
    std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszDefaultIDptr(pwszDefaultID, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });

    return std::string(CW2A(pwszDefaultID));
}

std::string WinApiWrapper::GetOutputDeviceName(IMMDevice* device) {
    qDebug() << "WinApiWrapper::GetOutputDeviceName";

    HRESULT hr = S_OK;

    IPropertyStore* pProps = nullptr;
    if (hr = device->OpenPropertyStore(STGM_READ, &pProps); FAILED(hr)) {
        qWarning() << "OpenPropertyStore failed";
        return "";
    }
    winapi_unique_ptr<IPropertyStore> propsPtr(pProps, safeRelease<IPropertyStore>);

    PROPVARIANT varName;
    PropVariantInit(&varName);
    if (hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName); FAILED(hr)) {
        qWarning() << "device property GetValue call failed";
        return "";
    }

    if (varName.vt == VT_EMPTY) {
        qWarning() << "couldn't get device friendly name";
        return "";
    }

    std::string name = std::string(CW2A(varName.pwszVal));
    PropVariantClear(&varName);

    return name;
}


bool WinApiWrapper::RegisterGlobalShortcut(HotKeys hotKey) {
    qDebug() << "WinApiWrapper::RegisterGlobalShortcut";

    const auto hotKeyInfo = mHotKeys[static_cast<unsigned int>(hotKey)];
    if (!RegisterHotKey(NULL, 0, hotKeyInfo.mod, hotKeyInfo.key))
    {
        auto res = GetLastError();
        auto hres = HRESULT_FROM_WIN32(res);
        qWarning() << "couldn't register hotkey ALT+CTRL+M" << " last error: " << res << " hresult: " << hres;
        return false;
    }

    return true;
}

HotKeys WinApiWrapper::ParseHotKeyMessageParam(long long lParam) {
    auto highWord = HIWORD(lParam);
    auto lowWord = LOWORD(lParam);

    for (auto i = 0; i < mHotKeys.size(); i++) {
        if (lowWord == mHotKeys[i].mod && highWord == mHotKeys[i].key) {
            return static_cast<HotKeys>(i);
        }
    }

    return HotKeys::UKNOWN;
}

const std::vector<WinApiWrapper::HotKeyInfo> WinApiWrapper::mHotKeys = {
    { 0, VK_F13 }, // CYCLE_AUDIO_OUTPUTS
};
