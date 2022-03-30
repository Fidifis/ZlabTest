#pragma once
#include "headers.hpp"
#include "taskManager.hpp"
#include "taskTest.hpp"
#include "result.hpp"
#include "exitCodes.hpp"

CompileResult compile(const TaskTest *task, const char sourceCodeFile[]);

map<const string, ExitCode> runProgram(const TaskTest *task);

void compileAndRun(const TaskManager *task, const char sourceCodeFile[]);
