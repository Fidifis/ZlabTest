#pragma once
#include "headers.hpp"
#include "task.hpp"

int compile(const Task *task, const char sourceCodeFile[]);

int runProgram(const Task *task);
