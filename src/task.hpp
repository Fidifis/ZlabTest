#pragma once
#include "headers.hpp"
#include "stringSubstitution.cpp" //TODO: vyřešit aby fungoval hpp

using namespace nlohmann;

class Task
{
private:
    string taskName = "unnamed";
    string maxTime = "3";
    string compileArgs = "-Wall -pedantic";

    string inputData = "./$(taskName)/input/";
    string referenceData = "./$(taskName)/reference/";
    string playground = "./playground/";
    string outputData = "$(playground)/$(taskName)/ouput/";
    string outputBinaryFile = "$(playground)/$(taskName)/out.bin";
    string outputErrorsFile = "$(playground)/$(taskName)/errs";
    string outputRunTimeFile = "$(playground)/$(taskName)/time";

public:
    Task(const char path[], const char configPath[]);

    void runTests(const char sourceCodeFile[]) const;

private:
    void loadParameters(const json &js);

    void substituteNames();
    inline bool __substituteNames(string &arg);

public:
    const string& getTaskName() const { return taskName; }
    const string& getMaxTime() const { return maxTime; }
    const string& getCompileArgs() const { return compileArgs; }
    const string& getInputData() const { return inputData; }
    const string& getReferenceData() const { return referenceData; }
    const string& getPlayground() const { return playground; }
    const string& getOutputData() const { return outputData; }
    const string& getOutputBinaryFile() const { return outputBinaryFile; }
    const string& getOutputErrorsFile() const { return outputErrorsFile; }
    const string& getOutputRunTimeFile() const { return outputRunTimeFile; }
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
