#pragma once
#include "headers.hpp"
#include "exitCodes.hpp"
#include "taskTest.hpp"

class TaskTest;

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
private:
    ResultState state;

public:
    CompileResult compileResult;
    map<const string, ExitCode> unexpectedExitCodes;
    vector<string> differingOutput;

    json toJson() const;

    static void saveResult(const vector<TaskTest*> &tasks, const string &path);
};
