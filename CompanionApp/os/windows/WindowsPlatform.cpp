#include "WindowsPlatform.h"


using namespace osal;


osal::IPlatformPtr osal::CreatePlatform() {
    return std::make_shared<WindowsPlatform>();
}


WindowsPlatform::WindowsPlatform() {}

bool WindowsPlatform::openWebsite(const std::string& address) {
    return false;
}

bool WindowsPlatform::incVolume() {
    return false;
}

bool WindowsPlatform::decVolume() {
    return false;
}

bool WindowsPlatform::toggleMute() {
    return false;
}
