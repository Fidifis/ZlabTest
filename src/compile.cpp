#include "compile.hpp"

int compile(const Task *task, const string &sourceCodeFile)
{
    if (sourceCodeFile.empty())
    {
        cerr << "Path is empty. " << sourceCodeFile << endl;
        return 1;
    }
    if (filesystem::is_directory(sourceCodeFile))
    {
        cerr << "Given file is a directory. " << sourceCodeFile << endl;
        return 1;
    }

    cout << "Compiling source code from: " << sourceCodeFile << endl;

    string binFile = task->config->pathToPlayground + task->config->outputBinaryFileName;
    string errFile = task->config->pathToPlayground + task->config->compileStdErrFileName;

    //g++ [args] -o [out.bin] [x.cpp] 2> [err]
    const string cmd = "g++ " + task->compileArgs +
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

int runProgram(const Test *test)
{
    if (!filesystem::exists(test->testsInputPath))
    {
        cerr << "Path does not exists. " <<  test->testsInputPath << endl;
        return 1;
    }

    cout << "Executing program with inputs from: " << test->testsInputPath << endl;

    for (auto &file : filesystem::directory_iterator(test->testsInputPath)) {
        if (!file.exists() || file.is_directory())
            continue;

        cout << "Test " << file.path().filename().string() << endl;

        //( time -f '%E' timeout [x] cat [file] | [bin] > [output] ) > [time]
        const string cmd = "( time -f '%E' timeout " + test->task->maxTime +
            " cat " + file.path().string() +
            " | " + test->task->config->pathToPlayground + test->task->config->outputBinaryFileName +
            " > " + test->task->config->outputDataPath + file.path().filename().string() +
            "_out ) 2> " + test->task->config->outputDataPath + file.path().filename().string() + "_out_time";
        system(cmd.c_str());
    }

    return 0;
}
