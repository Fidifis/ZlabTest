#pragma once
#include "headers.hpp"
#include "exitCodes.hpp"

enum class CompileResult {none, success, warrings, fail};
inline const char* compileResultToString(CompileResult cr)
{
    switch(cr)
    {
        case CompileResult::none: return "none";
        case CompileResult::success: return "success";
        case CompileResult::warrings: return "warrings";
        case CompileResult::fail: return "fail";
        default: return "unknown";
    }
}

class Result
{
public:
    CompileResult compileResult;
    map<const string, ExitCode> runExitCodes;

    void save(const char *path) const;
};
