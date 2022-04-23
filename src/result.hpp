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

enum class ResultState { passed, failed, not_tested };
inline const char* resultStateToString(ResultState st)
{
    switch(st)
    {
        case ResultState::passed: return "passed";
        case ResultState::failed: return "failed";
        case ResultState::not_tested: return "not_tested";
        default: return "unknown";
    }
}

class Result
{
public:
    ResultState state = ResultState::not_tested;
    float successPercent = 0.f;
    int acquiredPoints = 0;

    CompileResult compileResult = CompileResult::none;
    map<const string, ExitCode> unexpectedExitCodes;
    vector<string> differingOutput;

    json toJson() const;
};
