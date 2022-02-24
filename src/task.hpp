#pragma once
#include "headers.hpp"
#include "config.hpp"

using namespace nlohmann;

class Task
{
public:
    string taskName;
    string testsPath;
    string compileArgs;
    string maxTime;
    Config *config;

public:
    Task(const Config *config, const string &path);

    void runTests(const char sourceCodeFile[]);
};
