#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "stringRef.hpp"

#define param paramHolder.paramStruct
#define paramArray paramHolder.paramArray_
#define TASK_NUMBER_OF_PARAMETERS 12

class Task
{
private:
    enum class ParamType {
        path = 1, containVariables = 2, specialLoad = 4
    };

    inline friend Flags operator| (ParamType p1, ParamType p2)
    { return (Flags)p1 | (Flags)p2; }
    inline friend Flags operator& (Flags p1, ParamType p2)
    { return p1 & (Flags)p2; }

    union ParamUnion
    {
        array<stringRef, TASK_NUMBER_OF_PARAMETERS> paramArray_;
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

    bool recompile = false;
    vector<Task*> tasks;

    Task(const Task *task, const string &testName, const nlohmann::json &js);

    inline void copy(const Task *task);
    void loadParameters(const nlohmann::json &js);

    void substituteAllNames();
    bool substituteNames(string &arg);

    inline void addSlashOnEnd(string &arg);
    
public:
    Task(const char path[], const char configPath[]);
    ~Task();

    const Task* operator[] (int index) const { return tasks[index]; }
    size_t size() const { return tasks.size(); }

    bool getRecompile() const { return recompile; }

    const string& getTaskName() const { return param.taskName.value; }
    const string& getTestName() const { return param.testName.value; }
    const string& getMaxTime() const { return param.maxTime.value; }
    const string& getCompileArgs() const { return param.compileArgs.value; }
    const string& getInputData() const { return param.inputData.value; }
    const string& getReferenceData() const { return param.referenceData.value; }
    const string& getPlayground() const { return param.playground.value; }
    const string& getOutputData() const { return param.outputData.value; }
    const string& getCompiledBinaryFile() const { return param.compiledBinaryFile.value; }
    const string& getCompileErrorsFile() const { return param.compileErrorsFile.value; }
    const string& getOutputErrors() const { return param.outputErrors.value; }
    const string& getOutputRunTime() const { return param.outputRunTime.value; }
};
