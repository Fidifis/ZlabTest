#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "result.hpp"

class TaskTest : public Task
{
private:
    bool recompile = false;

    void SpecialLoad(stringRef& item, const string& jsonValue) override;

public:
    TaskTest(const Task *task, const string &testName, const nlohmann::json &js);
    ~TaskTest();

    Result *result;

    inline bool getRecompile() const { return recompile; }
};
