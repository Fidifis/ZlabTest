#pragma once
#include "headers.hpp"
#include "config.hpp"

int compile(const string &sourceCodePath, const Config *conf);

int runProgram(const string &inputDataPath, const Config *conf);
