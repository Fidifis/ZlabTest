#pragma once
#include "headers.hpp"
#include "taskManager.hpp"
#include "taskTest.hpp"

enum class CompileResult {success, warrings, fail};

CompileResult compile(const TaskTest *task, const char sourceCodeFile[]);

void runProgram(const TaskTest *task);

void compileAndRun(const TaskManager *task, const char sourceCodeFile[]);
