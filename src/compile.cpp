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
    system(("g++ " + conf->compileArgs + " -o " + conf->pathToPlayground + conf->outputBinaryFileName + " " +
            sourceCodePath + " 2> " + conf->pathToPlayground + conf->compileStdErrFileName)
               .c_str());

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
