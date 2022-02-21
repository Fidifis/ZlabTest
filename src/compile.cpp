#include "compile.hpp"

int compile(const string &sourceCodePath, const Config *conf)
{
    if (sourceCodePath.empty())
    {
        cerr << "Path is empty.";
        return 1;
    }

    cout << "Compiling source code from: \n"
         << sourceCodePath << endl;

    const string cmd = "g++ " + conf->compileArgs +
            " -o " + conf->pathToPlayground + conf->outputBinaryFileName +
            " " + sourceCodePath +
            " 2> " + conf->pathToPlayground + conf->compileStdErrFileName;
    system(cmd.c_str());

    if (filesystem::exists(conf->pathToPlayground + conf->outputBinaryFileName))
    {
        if (filesystem::exists(conf->pathToPlayground + conf->compileStdErrFileName))
        {
            ifstream errfile(conf->pathToPlayground + conf->compileStdErrFileName);
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

int runProgram(const string &inputDataPath, const Config *conf)
{
    if (!filesystem::exists(inputDataPath))
        return 1;

    for (auto &file : filesystem::directory_iterator(inputDataPath)) {
        if (!file.exists())
            continue;

        //( time timeout x cat x | x > x ) > x
        const string cmd = "( time timeout " + conf->defaultTimeout +
            " cat " + file.path().string() +
            " | " + conf->pathToPlayground + conf->outputBinaryFileName +
            " > " + conf->outputDataPath + file.path().filename().string() +
            "_out ) 2> " + conf->outputDataPath + file.path().filename().string() + "_out_time";
        system(cmd.c_str());
    }

    return 0;
}
