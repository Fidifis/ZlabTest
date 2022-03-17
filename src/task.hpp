#pragma once
#include "headers.hpp"
#include "stringSubstitution.cpp" //TODO: vyřešit aby fungoval hpp
#include "compile.hpp"

class Task
{
private:
    string taskName = "unnamed";
    string testName = "global";
    string maxTime = "3";
    string compileArgs = "-Wall -pedantic";

    string inputData = "./$(taskName)/$(testName)/input/";
    string referenceData = "./$(taskName)/$(testName)/reference/";
    string playground = "./playground/";
    string outputData = "$(playground)/$(taskName)/$(testName)/ouput/";
    string compiledBinaryFile = "$(playground)/$(taskName)/out.bin";
    string compileErrorsFile = "$(playground)/$(taskName)/out.err";
    string outputErrors = "$(playground)/$(taskName)/$(testName)/errs/";
    string outputRunTime = "$(playground)/$(taskName)/$(testName)/time/";

    bool recompile = false;
    vector<Task*> tasks;

    Task(const Task *task, const string &testName, const nlohmann::json &js);

    void copy(const Task *task);
    void loadParameters(const nlohmann::json &js);

    void substituteNames();
    inline bool substituteNames(string &arg);

    inline void addSlashOnEnd(string &arg);
    
public:
    Task(const char path[], const char configPath[]);
    ~Task();

    void runTests(const char sourceCodeFile[]) const;

    const string& getTaskName() const { return taskName; }
    const string& getTestName() const { return testName; }
    const string& getMaxTime() const { return maxTime; }
    const string& getCompileArgs() const { return compileArgs; }
    const string& getInputData() const { return inputData; }
    const string& getReferenceData() const { return referenceData; }
    const string& getPlayground() const { return playground; }
    const string& getOutputData() const { return outputData; }
    const string& getCompiledBinaryFile() const { return compiledBinaryFile; }
    const string& getCompileErrorsFile() const { return compileErrorsFile; }
    const string& getOutputErrors() const { return outputErrors; }
    const string& getOutputRunTime() const { return outputRunTime; }
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
