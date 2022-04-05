#include "headers.hpp"
#include "taskManager.hpp"
#include "compileAndRun.hpp"

void clearFiles(const Task *task)
{
    for (auto &file : filesystem::directory_iterator(task->getPlayground())) 
        filesystem::remove_all(file.path());
}

void printHelp()
{
    cout << "usage: zlabtest [-d] [-c <config.json>] [-s <scripts>] <task.json> <source.cpp>" << endl;
}

inline char getOption(const char *argv)
{
    if (argv != nullptr && argv[0] == '-' && argv[1] != '\0' )
    {
        return argv[1];
    }
    else return '\0';
}

inline bool loadArguments(const int& argc, const char *const *const &argv,
    const char *&csourceCode , const char *&ctask,
    const char *&cconfig, const char *&cscripts)
{
    char option;
    uint8_t noOptArgsCount = 0;

    for (int i = 1; i < argc; ++i)
    {
        option = getOption(argv[i]);
        switch (option)
        {
            case '\0':
                switch(noOptArgsCount)
                {
                    case 0:
                        ctask = argv[i];
                    break;

                    case 1:
                        csourceCode = argv[i];
                    break;

                    default:
                        cerr << "Too many arguments" << endl;
                        return 1;
                }
                ++noOptArgsCount;
            break;

            case 'c':
                if (i + 1 < argc && getOption(argv[i + 1]) == '\0')
                {
                    cconfig = argv[i + 1];
                    ++i;
                }
                else
                {
                    cerr << "No argument for option " << option << endl;
                    printHelp();
                    return 1;
                }
            break;

            case 's':
                if (i + 1 < argc && getOption(argv[i + 1]) == '\0')
                {
                    cscripts = argv[i + 1];
                    ++i;
                }
                else
                {
                    cerr << "No argument for option " << option << endl;
                    printHelp();
                    return 1;
                }
            break;

            case 'd':
                //clearFiles();
            break;

            default:
                cerr << "Unknown option " << option << endl;
                printHelp();
                return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    const char *csourceCode = nullptr, *ctask = nullptr,
    *cconfig = nullptr, *cscripts = nullptr;

    bool returnVal =
    loadArguments(argc, argv, csourceCode, ctask, cconfig, cscripts);

    if (returnVal != 0)
        return returnVal;

    if (ctask == nullptr || csourceCode == nullptr)
    {
        cerr << "You need to specify path to task and source code" << endl;
        printHelp();
        return 1;
    }

    const TaskManager *task = new TaskManager(ctask, cconfig);

    try
    {
        compileAndRun(task, csourceCode);
        task->saveResultFile();
    }
    catch(const exception& e)
    {
        cerr << "FATAL ERROR: " << endl << e.what() << endl;
    }

    delete task;
    return 0;
}
