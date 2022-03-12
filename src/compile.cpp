#include "compile.hpp"

Result compile(const Task *task, const char sourceCodeFile[])
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

    //g++ [args] -o [out.bin] [x.cpp] 2> [err]
    const string cmd = "g++ " + task->getCompileArgs() +
            " -o " + binFile +
            " " + sourceCodeFile +
            " 2> " + errFile;
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
                return Result::success;
            }
            else
            {
                cout << "Compiled with warnings." << endl;
                return Result::warrings;
            }
        }
        else
        {
            cout << "Compiled successfully." << endl;
            return Result::success;
        }
    }
    else
    {
        cout << "Compilation failed." << endl;
        return Result::fail;
    }
}

void runProgram(const Task *task)
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

    for (auto &file : filesystem::directory_iterator(inputData)) {
        if (!file.exists() || file.is_directory())
            continue;

        const string filename = file.path().filename().string();

        cout << task->getTaskName() << ": " << task->getTestName() << ": " << filename << endl;

        //( time -f '%E' timeout [x] cat [file] | [bin] > [output] 2> [errors] ) 2> [time]
        const string cmd = "( time -f '%E' timeout " + task->getMaxTime() +
            " cat " + file.path().string() +
            " | " + task->getCompiledBinaryFile() +
            " > " + task->getOutputData() + filename + "_out" +
            " 2> "+ task->getOutputErrors() + filename + "_err" +
            " ) 2> " + task->getOutputRunTime() + filename + "_time";
        system(cmd.c_str());
    }
}
