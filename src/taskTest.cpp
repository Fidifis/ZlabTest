#include "taskTest.hpp"

TaskTest::TaskTest(const Task *task, const string &testName, const nlohmann::json &js)
{
    copy(task);
    PARAM.testName = testName;
    loadParameters(js);
    substituteAllNames();
}

void TaskTest::SpecialLoad(stringRef& item, const string& jsonValue) {
    Task::SpecialLoad(item, jsonValue);

    if (item.key == PARAM.compileArgs.key)
    {
        if (PARAM.compileArgs.value != jsonValue)
        {
            recompile = true;
            PARAM.compileArgs = jsonValue;
        }
    }
}
