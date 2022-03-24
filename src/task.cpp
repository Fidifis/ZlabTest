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

cout << "tady lol" << endl;
//cout << param.taskName.key << endl;
//cout << "turutu" << endl;

    //Fill class atributes with values from json
    if (js.contains(param.taskName.key))
        param.taskName.value = js[param.taskName.key];

    if (haveConf)
        loadParameters(conf);

    if (js.contains("shared"))
    {
        cout << "tady 2 lulw" << endl;
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
            cout << "dalo se to tam" << endl;
        }
    }
}

Task::Task(const Task *task, const string &testName, const json &js)
{
    cout << "2 konstruktor" << endl;
    copy(task);
    cout << "brambora" << endl;
    param.testName.value = testName;
cout << "okurka" << endl;
    loadParameters(js);
    cout << "salát" << endl;
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
    cout << "load paramansas" << endl;
    for (auto &item : paramArray)
    {
        cout << "trutrurtu" << endl;
        if (js.contains(item.key)) {
            if (item.flags & ParamType::specialLoad &&
                item.key == param.compileArgs.key)
            {
                cout << "lol lol" << endl;
                const string &_compileArgs = js[param.compileArgs.key];
                cout << "lulw lol" << endl;
                if (param.compileArgs.value == _compileArgs)
                {
                    recompile = true;
                    cout << "kokokokokoko" << endl;
                    param.compileArgs.value = _compileArgs;
                }
                cout << "tady omegalul" << endl;
            }
            else
            {
                cout << "profesionalni debugovani" << endl;
                item.value = js[item.key];
                cout << "yes" << endl;
                if (item.flags & ParamType::path)
                {
                    cout << "rododendron" << endl;
                    addSlashOnEnd(item.value);
                }
                cout << "kulihrasek" << endl;
            }
        }
        cout << "you spin me round round" << endl;
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
                substituteNames(item.value))
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
                substitute(arg, item.value, start, length);
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