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
    prerequisite = task->prerequisite;
    acquirablePoints = task->acquirablePoints;
    requiredPercentage = task->requiredPercentage;
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
                    SpecialLoad(item, js);
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

    if (js.contains(prerequisite.key))
    {
        prerequisite = js[prerequisite.key].get<vector<string>>();
    }
}

void Task::SpecialLoad(reflective<string> &item, const nlohmann::json &json) {
    if (item.key == PARAM.compileArgs.key)
    {
        if (PARAM.compileArgs.value != json[item.key])
        {
            recompile = true;
            item = json[item.key];
        }
    }
    else if (item.key == PARAM.acquirablePoints.key)
    {
        item = json[item.key];
        acquirablePoints = stoi(item);
    }
    else if (item.key == PARAM.requiredPercentage.key)
    {
        item = json[item.key];
        requiredPercentage = stof(item);
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
