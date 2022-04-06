#pragma once
#include "headers.hpp"
#include "taskManager.hpp"
#include "taskTest.hpp"
#include "result.hpp"
#include "exitCodes.hpp"

constexpr char SCRIPTS_FOLDER[] = "./scripts/";
constexpr char SCRIPT_RUN_PROGRAM_NAME[] = "runProgram.bash";

void compileAndRun(const TaskManager *task, const char *sourceCodeFile, const char *scriptsFolder);
