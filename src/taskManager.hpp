#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "taskTest.hpp"
#include "result.hpp"

class TaskManager
{
private:
    Task *globalTaskConfig;
    vector<TaskTest*> tasks;

public:
    TaskManager(const char path[], const char configPath[] = "");
    ~TaskManager();

    void saveResultFile() const;

    inline const TaskTest* operator[] (int index) const { return tasks[index]; }
    inline size_t size() const { return tasks.size(); }

    inline const Task * getGlobalTaskConfig() const { return globalTaskConfig; }
};