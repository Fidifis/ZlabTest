#include "task.hpp"

Task::Task(const char path[], const char configPath[])
{
    json js, conf;
    bool haveConf = false;

    //load task json file
    if (filesystem::exists(path) &&
        !filesystem::is_directory(path))
    {
        ifstream stream(path);
        stream >> js;
        stream.close();
    }

    //load config json file
    if (filesystem::exists(configPath) &&
        !filesystem::is_directory(configPath))
    {
        ifstream confStream(configPath);
        confStream >> conf;
        confStream.close();
        haveConf = true;
    }

    //Fill class atributes with values from json
    if (js.contains("taskName"))
        taskName = js["taskName"];

    if (haveConf)
        loadParameters(conf);

    if (js.contains("shared"))
    {
        json shared = js["shared"];
        loadParameters(shared);
    }

    substituteNames();
}

void Task::loadParameters(const json &js)
{
    if (js.contains("maxTime"))
        maxTime = js["maxTime"];

    if (js.contains("compileArgs"))
        compileArgs = js["compileArgs"];

    if (js.contains("inputData"))
        inputData = js["inputData"];

    if (js.contains("referenceData"))
        referenceData = js["referenceData"];

    if (js.contains("playground"))
        playground = js["playground"];

    if (js.contains("outputData"))
        outputData = js["outputData"];

    if (js.contains("outputBinaryFile"))
        outputBinaryFile = js["outputBinaryFile"];

    if (js.contains("outputErrorsFile"))
        outputErrorsFile = js["outputErrorsFile"];

    if (js.contains("outputRunTimeFile"))
        outputRunTimeFile = js["outputRunTimeFile"];
}

void Task::substituteNames() {
    bool changed = true;
    while (changed)
    {
        changed = false;
        
        if (__substituteNames(inputData))
            changed = true;
        
        if (__substituteNames(referenceData))
            changed = true;
        
        if (__substituteNames(playground))
            changed = true;
        
        if (__substituteNames(outputData))
            changed = true;
        
        if (__substituteNames(outputBinaryFile))
            changed = true;
        
        if (__substituteNames(outputErrorsFile))
            changed = true;
        
        if (__substituteNames(outputRunTimeFile))
            changed = true;
    }
}

inline bool Task::__substituteNames(string &arg)
{
    size_t start, length;
    string name;

    if (getSubstVarName(arg, name, start, length))
    {
        if (name.compare("inputData"))
            substitute(arg, inputData, start, length);

        else if (name.compare("referenceData"))
            substitute(arg, referenceData, start, length);

        else if (name.compare("playground"))
            substitute(arg, playground, start, length);

        else if (name.compare("outputData"))
            substitute(arg, outputData, start, length);

        else if (name.compare("outputBinaryFile"))
            substitute(arg, outputBinaryFile, start, length);

        else if (name.compare("outputErrorsFile"))
            substitute(arg, outputErrorsFile, start, length);

        else if (name.compare("outputRunTimeFile"))
            substitute(arg, outputRunTimeFile, start, length);

        else return false;

        return true;
    }
    else return false;
}
