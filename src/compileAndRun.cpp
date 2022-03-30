#include "compileAndRun.hpp"

CompileResult compile(const TaskTest *task, const char sourceCodeFile[])
{
    if (sourceCodeFile[0] == '\0')
    {
        throw invalid_argument("source code file path is empty");
    }
    if (!filesystem::exists(sourceCodeFile))
    {
        throw invalid_argument("source code file does not exists");
    }
    if (filesystem::is_directory(sourceCodeFile))
    {
        throw invalid_argument("source code file is a directory");
    }

    cout << "Compiling source code from: " << sourceCodeFile << endl;

    const string &binFile = task->getCompiledBinaryFile();
    const string &errFile = task->getCompileErrorsFile();

    //g++ --args-- -o '--out.bin--' '--x.cpp--' 2> '--err--'
    const string cmd = "g++ " + task->getCompileArgs() +
            " -o '" + binFile +
            "' '" + sourceCodeFile +
            "' 2> '" + errFile + "'";
    int returnCode = system(cmd.c_str());

    if (returnCode == 0 && filesystem::exists(binFile))
    {
        if (filesystem::exists(errFile))
        {
            ifstream errfile(errFile);
            string content;
            getline(errfile, content, '\0');
            errfile.close();

            if (content.empty())
            {
                cout << "Compiled successfully." << endl;
                return CompileResult::success;
            }
            else
            {
                cout << "Compiled with warnings." << endl;
                return CompileResult::warrings;
            }
        }
        else
        {
            cout << "Compiled successfully." << endl;
            return CompileResult::success;
        }
    }
    else
    {
        cout << "Compilation failed." << endl;
        return CompileResult::fail;
    }
}

map<const string, ExitCode> runProgram(const TaskTest *task)
{
    const string &inputData = task->getInputData();
    if (!filesystem::exists(inputData))
    {
        throw invalid_argument("Path does not exists: " + inputData);
    }
    if (!filesystem::is_directory(inputData))
    {
        throw invalid_argument("Path is not directory: " + inputData);
    }

    cout << "Executing program with inputs from: " << inputData << endl;

    map<const string, ExitCode> exitCodes;

    for (auto &file : filesystem::directory_iterator(inputData)) {
        if (!file.exists() || file.is_directory())
            continue;

        const string filename = file.path().filename().string();

        cout << task->getTaskName() << ": " << task->getTestName() << ": " << filename << endl;

        //( time -f '%E' timeout --time-- cat '--file--' | '--bin--' > '--output--' 2> '--errors--' ) 2> '--timeFile--'
        const string cmd = "( time -f '%E' timeout " + task->getMaxTime() +
            " cat '" + file.path().string() +
            "' | '" + task->getCompiledBinaryFile() +
            "' > '" + task->getOutputData() + filename + "_out" +
            "' 2> '"+ task->getOutputErrors() + filename + "_err" +
            "' ) 2> '" + task->getOutputRunTime() + filename + "_time'";
        const int exitCode = system(cmd.c_str());

        if (exitCode > MAX_EXPECTED_CODE)
        {
            exitCodes[filename] = exitCode;
        }
    }

    return exitCodes;
}

void compileAndRun(const TaskManager *task, const char sourceCodeFile[])
{
    bool compiled = false;
    for (size_t i = 0; i < task->size(); ++i)
    {
        const TaskTest *t = (*task)[i];
        compiled = !t->getRecompile() && compiled;
        if (!compiled)
        {
            t->result->compileResult = compile(t, sourceCodeFile);
        }
        else
        {
            t->result->compileResult = CompileResult::none;
        }

        t->result->runExitCodes = runProgram(t);
        compiled = !t->getRecompile();
    }
}
