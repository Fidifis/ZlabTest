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

    map<const string, size_t> imap;
    //make TaskTests
    for (const auto &item : js.items())
    {
        if (item.key() != PARAMETER_SHARED_STR &&
            item.key() != PARAMETER_TASK_NAME_STR)
        {
            const json &testJS = item.value();
            TaskTest *testTask = new TaskTest(globalTaskConfig, item.key(), testJS);
            imap[testTask->getTestName()] = tasks.size();
            tasks.push_back(testTask);
        }
    }
    mapIndexes(imap);
}

TaskManager::~TaskManager()
{
    delete globalTaskConfig;
    for (TaskTest *t : tasks)
    {
        delete t;
    }
}

void TaskManager::mapIndexes(const map<const string, size_t> &imap) const
{
    for (auto &item : tasks)
    {
        item->mapIndex(imap);
    }
}

void TaskManager::run(const char *sourceCodeFile) const
{
    bool compile = false;
    bool pass = true;
    int loopProtect = 100;
    while (pass)
    {
        --loopProtect;
        if (loopProtect <= 0)
        {
            throw runtime_error("loop protect in taskMan");
        }

        pass = false;
        for (size_t i = 0; i < tasks.size(); ++i)
        {
            if (tasks[i]->result->state != ResultState::not_tested)
                continue;

            bool doBreak = false;
            for (const auto &item : tasks[i]->getPrerequisiteIndexes())
            {
                if (tasks[item]->result->state != ResultState::passed)
                {
                    doBreak = true;
                    break;
                }
            }
            if (doBreak)
                continue;

            pass = true;

            compileAndRun(tasks[i], compile, sourceCodeFile);
            difference(tasks[i]);

            if (tasks[i]->result->successPercent >= tasks[i]->getRequiredPercentage())
                tasks[i]->result->state = ResultState::passed;

            else
                tasks[i]->result->state = ResultState::failed;

            tasks[i]->result->acquiredPoints = tasks[i]->getAcquirablePoints() * (tasks[i]->result->successPercent / 100.f);
        }
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

    int totalPoints = 0;
    bool haveWarnings = false;
    for (const TaskTest *test : tasks)
    {
        if (test->result->compileResult == CompileResult::warrings)
            haveWarnings = true;
        js["test results"][test->getTestName()] = test->result->toJson();
        totalPoints += test->result->acquiredPoints;
    }

    if (haveWarnings)
    {
        totalPoints -= totalPoints * (tasks[0]->getWarningPenaltyPercentage() / 100.f);
        js["warning penality applied"] = true;
    }
    js["total points"] = totalPoints;

    stream << setw(4) << js;
    stream.close();
}