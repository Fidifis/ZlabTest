#pragma once
#include "headers.hpp"
#include "stringSubstitution.hpp"
#include "stringRef.hpp"

//----------keep this updated---------------
#define TASK_NUMBER_OF_PARAMETERS 17
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
        array<reflective<string>, TASK_NUMBER_OF_PARAMETERS> paramArray;
        struct ParamStruct {
            
            StringRef(PARAMETER_TASK_NAME_SYMBOL, "unnamed", (Flags)ParamType::manualLoad);
            StringRef(testName, "global", (Flags)ParamType::manualLoad);
            StringRef(maxTime, "3", 0);
            StringRef(compileArgs, "-std=c++17 -O2", (Flags)ParamType::specialLoad);

            StringRef(inputData, "./tasks/$(taskName)/$(testName)/input/", ParamType::path | ParamType::containVariables);
            StringRef(referenceData, "./tasks/$(taskName)/$(testName)/reference/", ParamType::path | ParamType::containVariables);
            StringRef(playground, "./playground/", ParamType::path | ParamType::containVariables);
            StringRef(outputData, "$(playground)/$(taskName)/$(testName)/ouput/", ParamType::path | ParamType::containVariables);
            StringRef(compiledBinaryFile, "$(playground)/$(taskName)/out.bin", (Flags)ParamType::containVariables);
            StringRef(compileErrorsFile, "$(playground)/$(taskName)/compile_errors", (Flags)ParamType::containVariables);
            StringRef(outputErrors, "$(playground)/$(taskName)/$(testName)/errors/", ParamType::path | ParamType::containVariables);
            StringRef(outputRunTime, "$(playground)/$(taskName)/$(testName)/time/", ParamType::path | ParamType::containVariables);
            StringRef(differenceData, "$(playground)/$(taskName)/$(testName)/diff/", ParamType::path | ParamType::containVariables);
            StringRef(resultFile, "$(playground)/$(taskName)/result.json", (Flags)ParamType::containVariables);
            StringRef(requiredPercentage, "0", (Flags)ParamType::specialLoad);
            StringRef(acquirablePoints, "10", (Flags)ParamType::specialLoad);
            StringRef(warningPenaltyPercentage, "10", (Flags)ParamType::specialLoad);
        } paramStruct = ParamStruct();

        ParamUnion() { }
        ~ParamUnion() { paramStruct.~ParamStruct(); } //bez tohoto ????dku dojde k memory leaku
    } paramHolder;

    Reflective(vector<string>, prerequisite, 0);
    float requiredPercentage = 0.f;
    float warningPenaltyPercentage = 10.f;
    int acquirablePoints = 10;
    bool recompile = false;

    Task() { }

    void copy(const Task *task);
    void loadParameters(const nlohmann::json &js);

    void substituteAllNames();
    bool substituteNames(string &arg);

    inline void addSlashOnEnd(string &arg);

    void SpecialLoad(reflective<string>& item, const nlohmann::json &json);

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
    inline const vector<string>& getPrerequisite() const { return prerequisite.value; }
    inline float getRequiredPercentage() const { return requiredPercentage; }
    inline int getAcquirablePoints() const { return acquirablePoints; }
    inline float getWarningPenaltyPercentage() const { return warningPenaltyPercentage; }
    inline bool getRecompile() const { return recompile; }
};
