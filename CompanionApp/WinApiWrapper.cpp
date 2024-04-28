#include "WinApiWrapper.h"
#include "IPolicyConfig.h"

#include <windows.h>
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <atlstr.h>

#include <QDebug>

#include <memory>


template<typename T>
using winapi_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

template<typename T>
static void safeRelease(T* ptr) {
    if (ptr) {
        ptr->Release();
    }
}

OutputDevices WinApiWrapper::EnumerateOutputDevices() {
    qDebug() << "WinApiWrapper::EnumerateOutputDevices";

    OutputDevices devices;
    HRESULT hr = S_OK;

    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

    IMMDeviceEnumerator* pEnumerator = NULL;
    if (hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator); FAILED(hr)) {
        qWarning() << "CoCreateInstance: failed to create IID_IMMDeviceEnumerator";
        return {};
    }
    winapi_unique_ptr<IMMDeviceEnumerator> enumeratorPtr(pEnumerator, safeRelease<IMMDeviceEnumerator>);

    IMMDeviceCollection* pCollection = NULL;
    if (hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection); FAILED(hr)) {
        qWarning() << "EnumAudioEndpoints: failed";
        return {};
    }
    winapi_unique_ptr<IMMDeviceCollection> collectionPtr(pCollection, safeRelease<IMMDeviceCollection>);

    UINT count;
    if(hr = pCollection->GetCount(&count); FAILED(hr)) {
        qWarning() << "couldn't get collection device count";
        return {};
    }
    qDebug() << "found " << count << " devices";

    IMMDevice* pDefaultEndpoint = NULL;
    if (hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultEndpoint); FAILED(hr)) {
        qWarning() << "GetDefaultAudioEndpoint: failed";
        return {};
    }
    winapi_unique_ptr<IMMDevice> defaultEndpointPtr(pDefaultEndpoint, safeRelease<IMMDevice>);

    LPWSTR pwszDefaultID;
    if (hr = pDefaultEndpoint->GetId(&pwszDefaultID); FAILED(hr)) {
        qWarning() << "couldn't get default device id";
        return {};
    }
    std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszDefaultIDptr(pwszDefaultID, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });
    devices.defaultDeviceID = std::string(CW2A(pwszDefaultID));

    devices.endpoints.reserve(count);
    for (ULONG i = 0; i < count; i++) {
        IMMDevice* pEndpoint = NULL;
        if (hr = pCollection->Item(i, &pEndpoint); FAILED(hr)) {
            qWarning() << "couldn't get device from collection for index " << i;
            continue;
        }
        winapi_unique_ptr<IMMDevice> endPointPtr(pEndpoint, safeRelease<IMMDevice>);

        DWORD state;
        if (hr = pEndpoint->GetState(&state); FAILED(hr)) {
            qWarning() << "couldn't get device state for index " << i;
            continue;
        }

        if (state != DEVICE_STATE_ACTIVE) {
            qWarning() << "device not active for index " << i;
            continue;
        }

        LPWSTR pwszID;
        if (hr = pEndpoint->GetId(&pwszID); FAILED(hr)) {
            qWarning() << "couldn't get device id for index " << i;
            continue;
        }
        std::unique_ptr<wchar_t, void(*)(LPWSTR)> pwszIDptr(pwszID, [](LPWSTR ptr){ ::CoTaskMemFree(ptr); });

        IPropertyStore* pProps = NULL;
        if (hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps); FAILED(hr)) {
            qWarning() << "OpenPropertyStore failed for index " << i;
            continue;
        }
        winapi_unique_ptr<IPropertyStore> propsPtr(pProps, safeRelease<IPropertyStore>);

        PROPVARIANT varName;
        PropVariantInit(&varName);
        if (hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName); FAILED(hr)) {
            qWarning() << "device property GetValue call failed for index " << i;
            continue;
        }

        if (varName.vt == VT_EMPTY) {
            qWarning() << "couldn't get device friendly name for index " << i;
            continue;
        }

        devices.endpoints.emplace_back(i, std::string(CW2A(varName.pwszVal)), std::string(CW2A(pwszID)));
        PropVariantClear(&varName);
    }

    return devices;
}

bool WinApiWrapper::SetDefaultDevice(const std::string& id) {
    qDebug() << "WinApiWrapper::SetDefaultDevice";

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
