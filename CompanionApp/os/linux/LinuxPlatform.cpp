#include "LinuxPlatform.h"

#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace osal;


osal::IPlatformPtr osal::CreatePlatform() {
    return std::make_shared<LinuxPlatform>();
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
