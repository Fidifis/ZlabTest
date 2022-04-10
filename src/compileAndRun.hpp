#pragma once
#include "headers.hpp"
#include "taskManager.hpp"
#include "taskTest.hpp"
#include "result.hpp"
#include "exitCodes.hpp"
#include "scripts.hpp"

void compileAndRun(const TaskManager *task, const char *sourceCodeFile);
