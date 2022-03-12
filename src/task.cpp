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
    if (js.contains("taskName"))
        taskName = js["taskName"];

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
            item.key().compare("taskName") != 0)
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
    this->testName = testName;

    loadParameters(js);
    substituteNames();
}

Task::~Task()
{
    for (Task *t : tasks)
    {
        delete t;
    }
}

void Task::runTests(const char sourceCodeFile[]) const
{
    bool compiled = false;
    for (Task *t : tasks)
    {
        compiled = !t->recompile && compiled;
        if (!compiled)
            compile(t, sourceCodeFile);

        runProgram(t);
        compiled = !t->recompile;
    }
}

void Task::copy(const Task *task)
{
    taskName = task->taskName;
    maxTime = task->maxTime;
    compileArgs = task->compileArgs;
    inputData = task->inputData;
    referenceData = task->referenceData;
    playground = task->playground;
    outputData = task->outputData;
    compiledBinaryFile = task->compiledBinaryFile;
    compileErrorsFile = task->compileErrorsFile;
    outputErrorsFile = task->outputErrorsFile;
    outputRunTimeFile = task->outputRunTimeFile;
}

void Task::loadParameters(const json &js)
{
    if (js.contains("maxTime"))
        maxTime = js["maxTime"];

    if (js.contains("compileArgs"))
    {
        const string &_compileArgs = js["compileArgs"];
        if (compileArgs.compare(_compileArgs))
        {
            recompile = true;
            compileArgs = _compileArgs;
        }
    }
    if (js.contains("inputData"))
        inputData = js["inputData"];

    if (js.contains("referenceData"))
        referenceData = js["referenceData"];

    if (js.contains("playground"))
        playground = js["playground"];

    if (js.contains("outputData"))
        outputData = js["outputData"];

    if (js.contains("compiledBinaryFile"))
        compiledBinaryFile = js["compiledBinaryFile"];

    if (js.contains("compileErrorsFile"))
        compileErrorsFile = js["compileErrorsFile"];

    if (js.contains("outputErrorsFile"))
        outputErrorsFile = js["outputErrorsFile"];

    if (js.contains("outputRunTimeFile"))
        outputRunTimeFile = js["outputRunTimeFile"];
}

void Task::substituteNames() {
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
        
        if (substituteNames(inputData))
            changed = true;
        
        if (substituteNames(referenceData))
            changed = true;
        
        if (substituteNames(playground))
            changed = true;
        
        if (substituteNames(outputData))
            changed = true;
        
        if (substituteNames(compiledBinaryFile))
            changed = true;

        if (substituteNames(compileErrorsFile))
            changed = true;
        
        if (substituteNames(outputErrorsFile))
            changed = true;
        
        if (substituteNames(outputRunTimeFile))
            changed = true;
    }
}

inline bool Task::substituteNames(string &arg)
{
    size_t start, length;
    string name;

    if (getSubstVarName(arg, name, start, length))
    {
        if (!name.compare("taskName"))
            substitute(arg, taskName, start, length);

        else if (!name.compare("testName"))
            substitute(arg, testName, start, length);

        else if (!name.compare("maxTime"))
            substitute(arg, maxTime, start, length);

        else if (!name.compare("compileArgs"))
            substitute(arg, compileArgs, start, length);

        else if (!name.compare("inputData"))
            substitute(arg, inputData, start, length);

        else if (!name.compare("referenceData"))
            substitute(arg, referenceData, start, length);

        else if (!name.compare("playground"))
            substitute(arg, playground, start, length);

        else if (!name.compare("outputData"))
            substitute(arg, outputData, start, length);

        else if (!name.compare("compiledBinaryFile"))
            substitute(arg, compiledBinaryFile, start, length);

        else if (!name.compare("compileErrorsFile"))
            substitute(arg, compileErrorsFile, start, length);

        else if (!name.compare("outputErrorsFile"))
            substitute(arg, outputErrorsFile, start, length);

        else if (!name.compare("outputRunTimeFile"))
            substitute(arg, outputRunTimeFile, start, length);

        else return false;

        return true;
    }
    else return false;
}
