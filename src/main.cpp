#include "headers.hpp"
#include "task.hpp"
#include "compile.hpp"

const char CONFIG_FILE_NAME[] = "config.json";

void clearFiles(const Task *task)
{
    for (auto &file : filesystem::directory_iterator(task->getPlayground())) 
        filesystem::remove_all(file.path());
}

void printHelp()
{
    cout << "usage: zlabtest [-c] [-t] <task.json> <source.cpp> [<config.json>]" << endl;
}

inline char getOption(char *argv)
{
    if (argv != nullptr && argv[0] == '-' && argv[1] != '\0' )
    {
        return argv[1];
    }
    else return '\0';
}

int main(int argc, char *argv[])
{
    const char *csourceCode, *ctask,
    *cconfig = CONFIG_FILE_NAME;

    if (argc <= 2)
    {
        printHelp();
        return 1;
    }
    else if (argc == 3)
    {
        ctask = argv[1];
        csourceCode = argv[2];
    }
    else if (argc == 4)
    {
        ctask = argv[1];
        csourceCode = argv[2];
        cconfig = argv[3];
    }
    else
    {
        cerr << "Too many arguments." << endl;
        return 1;
    }

    Task *task = new Task(ctask, cconfig);

    try
    {
        task->runTests(csourceCode);
    }
    catch(const exception& e)
    {
        cerr << e.what() << endl;
    }

    delete task;
    return 0;
}
