#pragma once
#include "headers.hpp"
#include "task.hpp"
#include "result.hpp"

class TaskTest : public Task
{
private:
    vector<size_t> prerequisiteIndexes;
public:
    TaskTest(const Task *task, const string &testName, const nlohmann::json &js);
    ~TaskTest();

    Result *result;

    void mapIndex(const map<const string, size_t> imap);

    inline const vector<size_t> getPrerequisiteIndexes() const { return prerequisiteIndexes; }
};
