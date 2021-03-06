#pragma once
#include "headers.hpp"

constexpr char SCRIPT_COMPILE_NAME[] = "compile.bash";
constexpr char SCRIPT_RUN_PROGRAM_NAME[] = "runProgram.bash";
constexpr char SCRIPT_DIFFERENCE_NAME[] = "difference.bash";

class Scripts
{
private:
    inline static string scriptsDirectory = "./scripts/";
    inline static string compile = scriptsDirectory + SCRIPT_COMPILE_NAME;
    inline static string runProgram = scriptsDirectory + SCRIPT_RUN_PROGRAM_NAME;
    inline static string difference = scriptsDirectory + SCRIPT_DIFFERENCE_NAME;

public:
    static bool exists();
    static void setScriptDirectory(const char *direcotry, bool throw_ = true);

    inline static string getCompile()
    {
        return compile;
    }
    inline static string getRunProgram()
    {
        return runProgram;
    }
    inline static string getDifference()
    {
        return difference;
    }
};
