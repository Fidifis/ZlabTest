#include "task.hpp"

using namespace nlohmann;

Task::Task(const char path[], const char configPath[] = "")
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

    //Fill class atributes with values from json
    if (js.contains(param.taskName.key))
        param.taskName = js[param.taskName.key];

    if (haveConf)
        loadParameters(conf);

    if (js.contains("shared"))
    {
        const json &shared = js["shared"];
        loadParameters(shared);
    }

    //substituteNames();

    for (const auto &item : js.items())
    {
        if (item.key().compare("shared") != 0 &&
            item.key().compare(param.taskName.key) != 0)
        {
            const json &testJS = item.value();
            Task *testTask = new Task(this, item.key(), testJS);
            tasks.push_back(testTask);
        }
    }
}

Task::Task(const Task *task, const string &testName, const json &js)
{
    copy(task);
    param.testName = testName;

    loadParameters(js);
    substituteAllNames();
}

Task::~Task()
{
    for (Task *t : tasks)
    {
        delete t;
    }
}

void Task::copy(const Task *task)
{
    for (size_t i = 0; i < paramArray.size(); ++i)
    {
        paramArray[i] = task->paramArray[i];
    }
}

void Task::loadParameters(const json &js)
{
    for (auto &item : paramArray)
    {
        if (item.flags & ParamType::specialLoad &&
            item.key == param.compileArgs.key)
        {
            const string &_compileArgs = js[param.compileArgs.key];
            if (param.compileArgs.value == _compileArgs)
            {
                recompile = true;
                param.compileArgs = _compileArgs;
            }
        }
        else if (js.contains(item.key))
        {
            item = js[item.key];
            if (item.flags & ParamType::path)
            {
                addSlashOnEnd(item);
            }
        }
    }
}

void Task::substituteAllNames() {
    bool changed = true;
    int loopProtect = 100;
    while (changed)
    {
        changed = false;

        if (--loopProtect <= 0)
        {
            cerr << "Cyclical variable reference in json" << endl;
            break;
        }

        for (auto &item : paramArray)
        {
            if (item.flags & ParamType::containVariables &&
                substituteNames(item))
            {
                changed = true;
            }
        } 
    }
}

bool Task::substituteNames(string &arg)
{
    size_t start, length;
    string name;

    if (getSubstVarName(arg, name, start, length))
    {
        for (auto &item : paramArray)
        {
            if (name == item.key)
            {
                substitute(arg, item, start, length);
                return true;
            }
        }
    }
    return false;
}

void Task::addSlashOnEnd(string &arg) {
    if (arg.length() > 0 && arg[arg.length() - 1] != '/')
    {
        arg += '/';
    }
}