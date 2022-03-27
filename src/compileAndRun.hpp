#pragma once
#include "headers.hpp"
#include "task.hpp"

enum class CompileResult {success, warrings, fail};

CompileResult compile(const Task *task, const char sourceCodeFile[]);

void runProgram(const Task *task);

void compileAndRun(const Task *task, const char sourceCodeFile[]);
