#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "result.hpp"

class Result;

class TaskTest : public Task
{
public:
    TaskTest(const Task *task, const string &testName, const nlohmann::json &js);
    ~TaskTest();

    Result *result;
};
