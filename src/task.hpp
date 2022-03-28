#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "stringRef.hpp"

#define PARAM paramHolder.paramStruct
#define PARAM_ARRAY paramHolder.paramArray
#define TASK_NUMBER_OF_PARAMETERS 12

class TaskTest;

class Task
{
protected:
    enum class ParamType {
        path = 1, containVariables = 2, specialLoad = 4
    };

    inline friend Flags operator| (ParamType p1, ParamType p2)
    { return (Flags)p1 | (Flags)p2; }
    inline friend Flags operator& (Flags p1, ParamType p2)
    { return p1 & (Flags)p2; }

    union ParamUnion
    {
        array<stringRef, TASK_NUMBER_OF_PARAMETERS> paramArray;
        struct ParamStruct {
            StringRef(taskName, "unnamed", 0);
            StringRef(testName, "global", 0);
            StringRef(maxTime, "3", 0);
            StringRef(compileArgs, "-Wall -pedantic", (Flags)ParamType::specialLoad);

            StringRef(inputData, "./$(taskName)/$(testName)/input/", ParamType::path | ParamType::containVariables);
            StringRef(referenceData, "./$(taskName)/$(testName)/reference/", ParamType::path | ParamType::containVariables);
            StringRef(playground, "./playground/", ParamType::path | ParamType::containVariables);
            StringRef(outputData, "$(playground)/$(taskName)/$(testName)/ouput/", ParamType::path | ParamType::containVariables);
            StringRef(compiledBinaryFile, "$(playground)/$(taskName)/out.bin", (Flags)ParamType::containVariables);
            StringRef(compileErrorsFile, "$(playground)/$(taskName)/out.err", (Flags)ParamType::containVariables);
            StringRef(outputErrors, "$(playground)/$(taskName)/$(testName)/errs/", ParamType::path | ParamType::containVariables);
            StringRef(outputRunTime, "$(playground)/$(taskName)/$(testName)/time/", ParamType::path | ParamType::containVariables);
        } paramStruct = ParamStruct();

        ParamUnion() { }
        ~ParamUnion() { paramStruct.~ParamStruct(); } //bez tohoto řádku dojde k memory leaku
    } paramHolder;

    Task() { }

    void copy(const Task *task);
    void loadParameters(const nlohmann::json &js);

    void substituteAllNames();
    bool substituteNames(string &arg);

    inline void addSlashOnEnd(string &arg);

    virtual void SpecialLoad(stringRef& item, const string& jsonValue);

private:
    vector<TaskTest*> tasks;
    
public:
    Task(const char path[], const char configPath[]);
    virtual ~Task();

    const TaskTest* operator[] (int index) const { return tasks[index]; }
    size_t size() const { return tasks.size(); }

    const string& getTaskName() const { return PARAM.taskName.value; }
    const string& getTestName() const { return PARAM.testName.value; }
    const string& getMaxTime() const { return PARAM.maxTime.value; }
    const string& getCompileArgs() const { return PARAM.compileArgs.value; }
    const string& getInputData() const { return PARAM.inputData.value; }
    const string& getReferenceData() const { return PARAM.referenceData.value; }
    const string& getPlayground() const { return PARAM.playground.value; }
    const string& getOutputData() const { return PARAM.outputData.value; }
    const string& getCompiledBinaryFile() const { return PARAM.compiledBinaryFile.value; }
    const string& getCompileErrorsFile() const { return PARAM.compileErrorsFile.value; }
    const string& getOutputErrors() const { return PARAM.outputErrors.value; }
    const string& getOutputRunTime() const { return PARAM.outputRunTime.value; }
};

#include "taskTest.hpp"
