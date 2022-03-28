#include "taskManager.hpp"

TaskManager::TaskManager(const char path[], const char configPath[])
{
    json js, conf;
    bool haveConf = false;

    //load task json file
    if (filesystem::exists(path) &&
        !filesystem::is_directory(path))
    {
        cout << "loading given json file " << endl;
        ifstream stream(path);
        stream >> js;
        stream.close();
    }
    else
    {
        cerr << "failed load task json file." << endl;
        return;
    }

    //load config json file
    if (configPath[0] == '\0')
    {
        cout << "no config file specified" << endl;
    }
    else if (filesystem::exists(configPath) &&
        !filesystem::is_directory(configPath))
    {
        cout << "loading general configuration from config file" << endl;
        ifstream confStream(configPath);
        confStream >> conf;
        confStream.close();
        haveConf = true;
    }
    else
    {
        cerr << "config file not found" << endl;
    }

    globalTaskConfig = new Task(js, conf);

    assert(strcmp(PARAMETER_TASK_NAME_STR, "taskName") == 0);
    assert(strcmp(PARAMETER_SHARED_STR, "shared") == 0);

    //make TaskTests
    for (const auto &item : js.items())
    {
        if (item.key() != PARAMETER_SHARED_STR &&
            item.key() != PARAMETER_TASK_NAME_STR)
        {
            const json &testJS = item.value();
            TaskTest *testTask = new TaskTest(globalTaskConfig, item.key(), testJS);
            tasks.push_back(testTask);
        }
    }
}

TaskManager::~TaskManager()
{
    delete globalTaskConfig;
    for (TaskTest *t : tasks)
    {
        delete t;
    }
}