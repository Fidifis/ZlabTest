#include "test.hpp"

Test::Test(const Task *task, const string &path)
{
    json js;
    ifstream stream(path);
    stream >> js;
    stream.close();

    testName = js["testName"];
    testsInputPath = js["testsInputPath"];
    overrideCompileArgs = js["overrideCompileArgs"];
    overrideMaxTime = js["overrideMaxTime"];
}

string Test::getCompileArgs()
{
    if (overrideCompileArgs.empty())
        return task->compileArgs;
    else
        return overrideCompileArgs;
}

string Test::getMaxTime()
{
    if (overrideMaxTime.empty())
        return task->maxTime;
    else
        return overrideMaxTime;
}

void Test::runTests()
{
    
}
