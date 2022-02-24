#include "task.hpp"

Task::Task(const Config *config, const string &path)
{
    json js;
    ifstream stream(path);
    stream >> js;
    stream.close();

    taskName = js["taskName"];
    testsPath = js["testsPath"];
    compileArgs = js["compileArgs"];
    maxTime = js["maxTime"];
}

void Task::runTests(const char sourceCodeFile[])
{
    
}
