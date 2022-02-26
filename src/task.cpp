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
    
}
