#include "task.hpp"

Task::Task(const json& taskJson, const json* globalConfig)
{
    //Fill class atributes with values from json
    if (taskJson.contains(PARAM.taskName.key))
        PARAM.taskName = taskJson[PARAM.taskName.key];

    if (globalConfig != nullptr)
        loadParameters(*globalConfig);

    if (taskJson.contains(PARAMETER_SHARED_STR))
    {
        const json &shared = taskJson[PARAMETER_SHARED_STR];
        loadParameters(shared);
    }
}

void Task::copy(const Task *task)
{
    for (size_t i = 0; i < PARAM_ARRAY.size(); ++i)
    {
        PARAM_ARRAY[i] = task->PARAM_ARRAY[i];
    }
}

void Task::loadParameters(const json &js)
{
    recompile = false;
    for (auto &item : PARAM_ARRAY)
    {
        if (js.contains(item.key))
        {
            if (!(item.flags & ParamType::manualLoad))
            {
                if (item.flags & ParamType::specialLoad)
                {
                    SpecialLoad(item, js[item.key], js);
                }
                else
                {
                    item = js[item.key];
                }
            }
            if (item.flags & ParamType::path)
            {
                addSlashOnEnd(item);
            }
        }
    }
}

void Task::SpecialLoad(stringRef& item, const string& jsonValue, const nlohmann::json &json) {
    if (item.key == PARAM.compileArgs.key)
    {
        if (PARAM.compileArgs.value != jsonValue)
        {
            recompile = true;
            PARAM.compileArgs = jsonValue;
        }
    }
    if (item.key == PARAM.prerequisite.key)
    {
        PARAM.prerequisite = jsonValue;

        prerequisiteArr = json[PARAM.prerequisite.key].get<vector<string>>();
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

        for (auto &item : PARAM_ARRAY)
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
        for (auto &item : PARAM_ARRAY)
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
