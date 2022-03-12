#include "compile.hpp"

int compile(const Task *task, const char sourceCodeFile[])
{
    if (sourceCodeFile[0] == '\0')
    {
        cerr << "Path is empty: " << sourceCodeFile << endl;
        return 1;
    }
    if (!filesystem::exists(sourceCodeFile))
    {
        cerr << "File does not exists: " << sourceCodeFile << endl;
        return 1;
    }
    if (filesystem::is_directory(sourceCodeFile))
    {
        cerr << "Given file is a directory: " << sourceCodeFile << endl;
        return 1;
    }

    cout << "Compiling source code from: " << sourceCodeFile << endl;

    const string &binFile = task->getCompiledBinaryFile();
    const string &errFile = task->getCompileErrorsFile();

    //g++ [args] -o [out.bin] [x.cpp] 2> [err]
    const string cmd = "g++ " + task->getCompileArgs() +
            " -o " + binFile +
            " " + sourceCodeFile +
            " 2> " + errFile;
    system(cmd.c_str());

    if (filesystem::exists(binFile))
    {
        if (filesystem::exists(errFile))
        {
            ifstream errfile(errFile);
            string content;
            getline(errfile, content, '\0');

            if (content.empty())
            {
                cout << "Compiled successfully." << endl;
            }
            else
            {
                cout << "Compiled with warnings." << endl;
            }

            errfile.close();
        }
        else
        {
            cout << "Compiled successfully." << endl;
        }
    }
    else
    {
        cout << "Compilation failed." << endl;
    }

    return 0;
}

int runProgram(const Task *task)
{
    const string &inputData = task->getInputData();
    if (!filesystem::exists(inputData))
    {
        cerr << "Path does not exists: " <<  inputData << endl;
        return 1;
    }
    if (!filesystem::is_directory(inputData))
    {
        cerr << "Path is not directory: " <<  inputData << endl;
        return 1;
    }

    cout << "Executing program with inputs from: " << inputData << endl;

    for (auto &file : filesystem::directory_iterator(inputData)) {
        if (!file.exists() || file.is_directory())
            continue;

        const string filename = file.path().filename().string();

        cout << "Test: " << task->getTaskName() << ": " << filename << endl;

        //( time -f '%E' timeout [x] cat [file] | [bin] > [output] 2> [errors] ) 2> [time]
        const string cmd = "( time -f '%E' timeout " + task->getMaxTime() +
            " cat " + file.path().string() +
            " | " + task->getCompiledBinaryFile() +
            " > " + task->getOutputData() + filename + "_out" +
            " 2> "+ task->getOutputErrorsFile() + filename + "_err" +
            " ) 2> " + task->getOutputRunTimeFile() + filename + "_time";
        system(cmd.c_str());
    }

    return 0;
}
