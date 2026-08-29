#include "WindowsPlatform.h"


using namespace os;

os::IPlatformPtr os::CreatePlatform() {
    return std::make_shared<WindowsPlatform>();
}


WindowsPlatform::WindowsPlatform() {}


bool WindowsPlatform::openWebsite(const std::string& address) {
    return false;
}

bool WindowsPlatform::launch(const std::string& appName, const std::vector<std::string>& args, const std::string& workingDir) {
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

bool WindowsPlatform::switchOutput() {
    return false;
}
