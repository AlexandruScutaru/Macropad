#include "LinuxPlatform.h"

#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

#include <vector>

using namespace osal;


osal::IPlatformPtr osal::CreatePlatform() {
    return std::make_shared<LinuxPlatform>();
}


LinuxPlatform::LinuxPlatform() {
    if (!mAudio.initialize()) {
        std::printf("unable to initialize the audio component\n");
    }
}

bool LinuxPlatform::openWebsite(const std::string& address) {
    if (address.empty()) {
        return false;
    }

    pid_t pid = 0;
    const char* argv[] = {"xdg-open", address.c_str(), nullptr};
    int result = posix_spawnp(&pid, "xdg-open", nullptr, nullptr, const_cast<char* const*>(argv), environ);

    return result == 0;
}

bool LinuxPlatform::launch(const std::string& appName, const std::vector<std::string>& args) {
    if (appName.empty()) {
        return false;
    }

    std::vector<const char*> argv;
    argv.push_back(appName.c_str());
    for (const auto& arg : args) {
        argv.push_back(arg.c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = 0;
    int result = posix_spawnp(&pid, appName.c_str(), nullptr, nullptr, const_cast<char* const*>(argv.data()), environ);

    return result == 0;
}

bool LinuxPlatform::incVolume() {
    return mAudio.increaseVolume(2);
}

bool LinuxPlatform::decVolume() {
    return mAudio.decreaseVolume(2);
}

bool LinuxPlatform::toggleMute() {
    return mAudio.toggleMute();
}

bool LinuxPlatform::switchOutput() {
    return mAudio.switchOutput();
}
