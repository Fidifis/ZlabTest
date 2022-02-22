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
    float maxTime;
    string sourceCodeFile;

    void load(const string &path, const char sourceCodeFile[]);

    void runTests(const Config *config);
};
