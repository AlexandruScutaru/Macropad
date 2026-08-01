#include "LinuxPlatform.h"

#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>

using namespace osal;

template<typename T>
using Resource = std::unique_ptr<T, std::function<void(T*)>>;


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

bool LinuxPlatform::launch(const std::string& appName, const std::vector<std::string>& args, const std::string& workingDir) {
    if (appName.empty()) {
        return false;
    }

    std::vector<const char*> argv;
    argv.push_back(appName.c_str());
    for (const auto& arg : args) {
        argv.push_back(arg.c_str());
    }
    argv.push_back(nullptr);

    std::string wd = workingDir;
    if (wd.empty()) {
        const auto home = std::getenv("HOME");
        wd = home != NULL ? home : "";
    }

#if defined(__GLIBC__) && defined(__GLIBC_PREREQ) && __GLIBC_PREREQ(2, 29)
    posix_spawn_file_actions_t fileActions;
    posix_spawn_file_actions_init(&fileActions);
    auto _raiiFileActions = Resource<posix_spawn_file_actions_t>(&fileActions, [](const auto& ptr) { if (ptr) posix_spawn_file_actions_destroy(ptr); });

    if (!wd.empty()) {
        // change working directory for the process
        posix_spawn_file_actions_addchdir_np(&fileActions, wd.c_str());
    }

    // redirect stdio to /dev/null so they are not shared with current app's
    posix_spawn_file_actions_addopen(&fileActions, STDIN_FILENO, "/dev/null", O_RDONLY, 0);
    posix_spawn_file_actions_addopen(&fileActions, STDOUT_FILENO, "/dev/null", O_WRONLY, 0);
    posix_spawn_file_actions_addopen(&fileActions, STDERR_FILENO, "/dev/null", O_WRONLY, 0);

    posix_spawnattr_t attributes;
    posix_spawnattr_init(&attributes);
    auto _raiiAttributes = Resource<posix_spawnattr_t>(&attributes, [](const auto& ptr) { if (ptr) { posix_spawnattr_destroy(ptr); }});

    // start process in detached mode
    short flags = 0;
    flags |= POSIX_SPAWN_SETPGROUP;
    posix_spawnattr_setpgroup(&attributes, 0); // 0 => new process group, child becomes group leader
    posix_spawnattr_setflags(&attributes, flags);

    pid_t pid = 0;
    int result = posix_spawnp(&pid, appName.c_str(), &fileActions, &attributes, const_cast<char* const*>(argv.data()), environ);
#else
    assert(false && "Unsupported version of glibc, cannot addchdir");

    // maybe I will add a fallback alternative with the extra chdir step
#endif

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
