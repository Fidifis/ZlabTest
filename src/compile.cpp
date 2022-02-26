#include "compile.hpp"

int compile(const Task *task, const char sourceCodeFile[])
{
    if (sourceCodeFile[0] == '/0')
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

    const string& binFile = task->getOutputBinaryFile();
    const string& errFile = task->getOutputErrorsFile();

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
    if (!filesystem::exists(task->getInputData()))
    {
        cerr << "Path does not exists. " <<  task->getInputData() << endl;
        return 1;
    }

    cout << "Executing program with inputs from: " << task->getInputData() << endl;

    for (auto &file : filesystem::directory_iterator(task->getInputData())) {
        if (!file.exists() || file.is_directory())
            continue;

        cout << "Test " << file.path().filename().string() << endl;

        //( time -f '%E' timeout [x] cat [file] | [bin] > [output] ) > [time]
        const string cmd = "( time -f '%E' timeout " + task->getMaxTime() +
            " cat " + file.path().string() +
            " | " + task->getOutputBinaryFile() +
            " > " + task->getOutputData() + file.path().filename().string() +
            "_out ) 2> " + task->getOutputErrorsFile() + file.path().filename().string() + "_out_time";
        system(cmd.c_str());
    }

    return 0;
}
