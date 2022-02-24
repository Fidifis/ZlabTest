#pragma once
#include "headers.hpp"
#include "task.hpp"

class Test
{
public:
    string testName;
    string testsInputPath;
    string overrideCompileArgs;
    string overrideMaxTime;
    Task *task;

public:
    Test(const Task *task, const string &path);

    string getCompileArgs();
    string getMaxTime();

    void runTests();
};
