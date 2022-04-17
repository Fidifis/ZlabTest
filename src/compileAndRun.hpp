#pragma once
#include "headers.hpp"
#include "taskTest.hpp"
#include "result.hpp"
#include "exitCodes.hpp"
#include "scripts.hpp"

void compileAndRun(const TaskTest *task, bool &compiled, const char *sourceCodeFile);
