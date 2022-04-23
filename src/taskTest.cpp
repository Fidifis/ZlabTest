#include "taskTest.hpp"

TaskTest::TaskTest(const Task *task, const string &testName, const nlohmann::json &js)
{
    result = new Result();
    copy(task);
    PARAM.testName = testName;
    loadParameters(js);
    substituteAllNames();
}

TaskTest::~TaskTest()
{
    delete result;
}

void TaskTest::mapIndex(const map<const string, size_t> imap)
{
    for (auto &item : prerequisite.value)
    {
        prerequisiteIndexes.push_back(imap.at(item));
    }
}