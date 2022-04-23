#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "taskTest.hpp"
#include "result.hpp"
#include "compileAndRun.hpp"
#include "difference.hpp"

constexpr char CONFIG_FILE_NAME[] = "config.json";

class TaskManager
{
private:
    Task *globalTaskConfig;
    vector<TaskTest*> tasks;

    void saveResult(const vector<TaskTest*> &tasks, const string &path) const;
    void mapIndexes(const map<const string, size_t> &imap) const;

public:
    TaskManager(const char path[], const char configPath[] = "");
    ~TaskManager();

    void run(const char *sourceCodeFile) const;

    void saveResultFile() const;
};