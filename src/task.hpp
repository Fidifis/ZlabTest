#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "stringRef.hpp"

//----------keep this updated---------------
#define TASK_NUMBER_OF_PARAMETERS 14
#define PARAMETER_TASK_NAME_SYMBOL taskName
#define PARAMETER_SHARED_SYMBOL shared
//------------------------------------------

#define PARAM paramHolder.paramStruct
#define PARAM_ARRAY paramHolder.paramArray
#define PARAMETER_TASK_NAME_STR MAKE_STR(PARAMETER_TASK_NAME_SYMBOL)
#define PARAMETER_SHARED_STR MAKE_STR(PARAMETER_SHARED_SYMBOL)

class Task
{
protected:
    enum class ParamType {
        path = 1, containVariables = 2, specialLoad = 4, manualLoad = 8
    };

    inline friend Flags operator| (ParamType p1, ParamType p2)
    { return (Flags)p1 | (Flags)p2; }
    inline friend Flags operator& (Flags p1, ParamType p2)
    { return p1 & (Flags)p2; }

    union ParamUnion
    {
        array<stringRef, TASK_NUMBER_OF_PARAMETERS> paramArray;
        struct ParamStruct {
            
            StringRef(PARAMETER_TASK_NAME_SYMBOL, "unnamed", (Flags)ParamType::manualLoad);
            StringRef(testName, "global", (Flags)ParamType::manualLoad);
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
            StringRef(differenceData, "$(playground)/$(taskName)/$(testName)/diff/", ParamType::path | ParamType::containVariables);
            StringRef(resultFile, "$(playground)/$(taskName)/result.json", (Flags)ParamType::containVariables);
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

public:
    Task(const json& taskJson, const json* globalConfig);
    virtual ~Task() { }

    inline const string& getTaskName() const { return PARAM.taskName.value; }
    inline const string& getTestName() const { return PARAM.testName.value; }
    inline const string& getMaxTime() const { return PARAM.maxTime.value; }
    inline const string& getCompileArgs() const { return PARAM.compileArgs.value; }
    inline const string& getInputData() const { return PARAM.inputData.value; }
    inline const string& getReferenceData() const { return PARAM.referenceData.value; }
    inline const string& getPlayground() const { return PARAM.playground.value; }
    inline const string& getOutputData() const { return PARAM.outputData.value; }
    inline const string& getCompiledBinaryFile() const { return PARAM.compiledBinaryFile.value; }
    inline const string& getCompileErrorsFile() const { return PARAM.compileErrorsFile.value; }
    inline const string& getOutputErrors() const { return PARAM.outputErrors.value; }
    inline const string& getOutputRunTime() const { return PARAM.outputRunTime.value; }
    inline const string& getDifferenceData() const { return PARAM.differenceData.value; }
    inline const string& getResultFile() const { return PARAM.resultFile.value; }
};
