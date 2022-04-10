#include "scripts.hpp"

bool Scripts::exists() {
    if (
        filesystem::exists(scriptsDirectory) && filesystem::is_directory(scriptsDirectory) &&
        filesystem::exists(getCompile()) && !filesystem::is_directory(getCompile()) &&
        filesystem::exists(getRunProgram()) && !filesystem::is_directory(getRunProgram()))
    return true;
    else return false;
}

void Scripts::setScriptDirectory(const char *direcotry, bool throw_) {
    scriptsDirectory = direcotry;
    compile = scriptsDirectory + '/' + SCRIPT_COMPILE_NAME;
    runProgram = scriptsDirectory + '/' + SCRIPT_RUN_PROGRAM_NAME;

    if (throw_ && !exists())
    {
        throw invalid_argument("scripts directory does not exists or does not contain all scripts");
    }
}
