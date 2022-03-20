#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "compile.hpp"
#include "stringRef.hpp"

#define TASK_NUMBER_OF_PARAMETERS 12

class Task
{
private:
    union
    {
        struct {
            StringRef(taskName, "unnamed");
            StringRef(testName, "global");
            StringRef(maxTime, "3");
            StringRef(compileArgs, "-Wall -pedantic");

            StringRef(inputData, "./$(taskName)/$(testName)/input/");
            StringRef(referenceData, "./$(taskName)/$(testName)/reference/");
            StringRef(playground, "./playground/");
            StringRef(outputData, "$(playground)/$(taskName)/$(testName)/ouput/");
            StringRef(compiledBinaryFile, "$(playground)/$(taskName)/out.bin");
            StringRef(compileErrorsFile, "$(playground)/$(taskName)/out.err");
            StringRef(outputErrors, "$(playground)/$(taskName)/$(testName)/errs/");
            StringRef(outputRunTime, "$(playground)/$(taskName)/$(testName)/time/");
        } param;

        array<stringRef, TASK_NUMBER_OF_PARAMETERS> paramArray;
    };
    
    bool recompile = false;
    vector<Task*> tasks;

    Task(const Task *task, const string &testName, const nlohmann::json &js);

    void copy(const Task *task);
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

    const string& getTaskName() const { return param.taskName; }
    const string& getTestName() const { return param.testName; }
    const string& getMaxTime() const { return param.maxTime; }
    const string& getCompileArgs() const { return param.compileArgs; }
    const string& getInputData() const { return param.inputData; }
    const string& getReferenceData() const { return param.referenceData; }
    const string& getPlayground() const { return param.playground; }
    const string& getOutputData() const { return param.outputData; }
    const string& getCompiledBinaryFile() const { return param.compiledBinaryFile; }
    const string& getCompileErrorsFile() const { return param.compileErrorsFile; }
    const string& getOutputErrors() const { return param.outputErrors; }
    const string& getOutputRunTime() const { return param.outputRunTime; }
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
