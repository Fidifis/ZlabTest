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
    param.taskName = task->param.taskName;
    param.maxTime = task->param.maxTime;
    param.compileArgs = task->param.compileArgs;
    param.inputData = task->param.inputData;
    param.referenceData = task->param.referenceData;
    param.playground = task->param.playground;
    param.outputData = task->param.outputData;
    param.compiledBinaryFile = task->param.compiledBinaryFile;
    param.compileErrorsFile = task->param.compileErrorsFile;
    param.outputErrors = task->param.outputErrors;
    param.outputRunTime = task->param.outputRunTime;
}

void Task::loadParameters(const json &js)
{
    if (js.contains("maxTime"))
        param.maxTime = js["maxTime"];

    if (js.contains("compileArgs"))
    {
        const string &_compileArgs = js["compileArgs"];
        if (param.compileArgs.value.compare(_compileArgs))
        {
            recompile = true;
            param.compileArgs = _compileArgs;
        }
    }
    if (js.contains("inputData"))
    {
        param.inputData = js["inputData"];
        addSlashOnEnd(param.inputData);
    }

    if (js.contains("referenceData"))
    {
        param.referenceData = js["referenceData"];
        addSlashOnEnd(param.referenceData);
    }

    if (js.contains("playground"))
    {
        param.playground = js["playground"];
        addSlashOnEnd(param.playground);
    }

    if (js.contains("outputData"))
    {
        param.outputData = js["outputData"];
        addSlashOnEnd(param.outputData);
    }

    if (js.contains("compiledBinaryFile"))
    {
        param.compiledBinaryFile = js["compiledBinaryFile"];
    }

    if (js.contains("compileErrorsFile"))
    {
        param.compileErrorsFile = js["compileErrorsFile"];
    }

    if (js.contains("outputErrors"))
    {
        param.outputErrors = js["outputErrors"];
        addSlashOnEnd(param.outputErrors);
    }

    if (js.contains("outputRunTime"))
    {
        param.outputRunTime = js["outputRunTime"];
        addSlashOnEnd(param.outputRunTime);
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
        
        if (substituteNames(param.inputData))
            changed = true;
        
        if (substituteNames(param.referenceData))
            changed = true;
        
        if (substituteNames(param.playground))
            changed = true;
        
        if (substituteNames(param.outputData))
            changed = true;
        
        if (substituteNames(param.compiledBinaryFile))
            changed = true;

        if (substituteNames(param.compileErrorsFile))
            changed = true;
        
        if (substituteNames(param.outputErrors))
            changed = true;
        
        if (substituteNames(param.outputRunTime))
            changed = true;
    }
}

bool Task::substituteNames(string &arg)
{
    size_t start, length;
    string name;

    if (getSubstVarName(arg, name, start, length))
    {
        if (!name.compare(param.taskName.key))
            substitute(arg, param.taskName, start, length);

        else if (!name.compare("testName"))
            substitute(arg, param.testName, start, length);

        else if (!name.compare("maxTime"))
            substitute(arg, param.maxTime, start, length);

        else if (!name.compare("compileArgs"))
            substitute(arg, param.compileArgs, start, length);

        else if (!name.compare("inputData"))
            substitute(arg, param.inputData, start, length);

        else if (!name.compare("referenceData"))
            substitute(arg, param.referenceData, start, length);

        else if (!name.compare("playground"))
            substitute(arg, param.playground, start, length);

        else if (!name.compare("outputData"))
            substitute(arg, param.outputData, start, length);

        else if (!name.compare("compiledBinaryFile"))
            substitute(arg, param.compiledBinaryFile, start, length);

        else if (!name.compare("compileErrorsFile"))
            substitute(arg, param.compileErrorsFile, start, length);

        else if (!name.compare("outputErrors"))
            substitute(arg, param.outputErrors, start, length);

        else if (!name.compare("outputRunTime"))
            substitute(arg, param.outputRunTime, start, length);

        else return false;

        return true;
    }
    else return false;
}

void Task::addSlashOnEnd(string &arg) {
    if (arg.length() > 0 && arg[arg.length() - 1] != '/')
    {
        arg += '/';
    }
}