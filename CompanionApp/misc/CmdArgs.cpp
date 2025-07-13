#include "CmdArgs.h"

#include <algorithm>
#include <utility>


CmdArgs::CmdArgs(int argc, char** argv, const std::vector<std::string>& args) {
    for (const auto& arg: args) {
        // TODO: extend this to differentiate from a flag and an option
        //       so that it can read the value as well, for now it looks only for flags
        if (isArgumentPresent(argv, argv + argc, arg)) {
            mParsedArgs.insert({ arg, "" });
        }
    }
}


bool CmdArgs::getFlag(const std::string& flag) {
    return mParsedArgs.find(flag) != mParsedArgs.end();
}

bool CmdArgs::isArgumentPresent(char** begin, char** end, const std::string& argument) {
    return std::find(begin, end, argument) != end;
}