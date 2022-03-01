#pragma once
#include "headers.hpp"
#include "task.hpp"

class Task;

int compile(const Task *task, const char sourceCodeFile[]);

int runProgram(const Task *task);
