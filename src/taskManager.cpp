#include "taskManager.hpp"

TaskManager::TaskManager(const char path[], const char configPath[])
{
    json js, conf;
    bool haveConf = false;

    if (path == nullptr)
        throw invalid_argument("task path is null");

    //load task json file
    if (filesystem::exists(path) &&
        !filesystem::is_directory(path))
    {
        cout << "loading given json file" << endl;
        ifstream stream(path);
        stream >> js;
        stream.close();
    }
    else
    {
        throw invalid_argument("failed load task json file");
        return;
    }

    //load config json file
    
    //this is to prevent calling the filesystem::exists() twice
    uint8_t state = 0;

    if (configPath == nullptr || configPath[0] == '\0')
    {
        if (filesystem::exists(CONFIG_FILE_NAME))
        {
            configPath = CONFIG_FILE_NAME;
            state = 1;
        }
        else
        {
            cout << "no config file specified" << endl;
            state = 2;
        }
    }
    if (state == 1 || (state == 0 && filesystem::exists(configPath) &&
        !filesystem::is_directory(configPath)))
    {
        cout << "loading general configuration from config file" << endl;
        ifstream confStream(configPath);
        confStream >> conf;
        confStream.close();
        haveConf = true;
    }
    else if (state != 2)
    {
        cerr << logcol("config file not found", Logcol::red) << endl;
    }

    globalTaskConfig = new Task(js, haveConf ? &conf : nullptr);

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

void TaskManager::run(const char *sourceCodeFile) const
{
    bool compile = false;
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        compileAndRun(tasks[i], compile, sourceCodeFile);
        difference(tasks[i]);
    }
}

void TaskManager::saveResultFile() const
{
    if (tasks.size() == 0)
    {
        cerr << "no tests to save the result" << endl;
        return;
    }
    saveResult(tasks, tasks[0]->getResultFile());
}

void TaskManager::saveResult(const vector<TaskTest*> &tasks, const string &path) const {
    cout << "Saving result file to: " << path << endl;
    ofstream stream(path);
    json js;

    for (const TaskTest *test : tasks)
    {
        js[test->getTestName()] = test->result->toJson();
    }

    stream << setw(4) << js;
    stream.close();
}