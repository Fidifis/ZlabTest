#pragma once
#include "headers.hpp"
#include "task.hpp"

class Task;

enum class Result {success, warrings, fail};

Result compile(const Task *task, const char sourceCodeFile[]);

void runProgram(const Task *task);
