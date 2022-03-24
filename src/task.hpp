#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "compile.hpp"
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
        
        ParamUnion() { /*paramStruct = ParamStruct();*/ }
        ~ParamUnion() {}
    } paramHolder = ParamUnion();
    
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

/*
sample json

{
    "taskName": "t",
    "shared": {
        "maxTime": "3",
        "inputData": "/tests_dir",
        "compileArgs": "-Wall -O2",
        "referenceData": "/refs_dir/"
        "outputData": "$(playgroud)/outs/"
        "playground": "/home/zlabtest/playgroud/",
        "outputBinaryFile": "$(playgroud)/out.bin",
        "outputErrFile": "$(playground)/bordel/err.txt",
        "outputRunTimeFile": "$(playground)/bordel/time.txt"
    },
    "test1": {
        "inputData": "/tests_dir/test1/",  <- each test must have defined folder with input data. This can be ommited when the parent folder is defined in the "shared" block
        "outputData": "$(playgroud)/outs/test1/,
        "referenceData": "/refs_dir/test1/",
        "compileArgs": "-Wall",
        "maxTime": "1.5"
        "outputBinaryFile": "$(playground)/compiled01.bin"
    },
    "test2": {
        "outputData": "/path/out2/",
        "referenceData": "/refs_dir/test2/"
    },
    "test3": {
        "compileArgs": "-Wall",
        "maxTime": "1.3"
    },
    "test4": {}
}

*/
