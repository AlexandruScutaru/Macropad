#pragma once

#include <string>
#include <vector>
#include <unordered_map>


class CmdArgs {
public:
    explicit CmdArgs(int argc, char** argv, const std::vector<std::string>& args);

    bool getFlag(const std::string& flag);

private:
    bool isArgumentPresent(char** begin, char** end, const std::string& argument);

    std::unordered_map<std::string, std::string> mParsedArgs;
};
