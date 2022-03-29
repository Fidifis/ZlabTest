#pragma once
#include "headers.hpp"
#include "exitCodes.hpp"

enum class CompileResult {none, success, warrings, fail};

class Result
{
public:
    CompileResult compileResult;
    vector<ExitCode> runExitCodes;
};
