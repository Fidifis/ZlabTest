#pragma once
#include "headers.hpp"

class Test
{
public:
    string testName;
    string testsInputPath;
    string overrideCompileArgs;
    string overrideMaxTime;

public:
    void load(const string &path);
};
