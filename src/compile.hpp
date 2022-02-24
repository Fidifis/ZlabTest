#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "test.hpp"

int compile(const Task *task, const string &sourceCodeFile);

int runProgram(const Test *test);
