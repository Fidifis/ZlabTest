#include "config.hpp"

void Config::load(const char path[])
{
    json js;
    toJSON(js);

    if (filesystem::exists(path))
    {
        ifstream configStream(path);
        json tmp;

        configStream >> tmp;
        configStream.close();

        js.merge_patch(tmp);
        parse(js);
    }
    else
    {
        cerr << "No config file. Creating new." << endl;
        ofstream configStream(path);
        configStream << setw(4) << js << endl;
        configStream.close();
    }
}

void Config::toJSON(json &js)
{
    js =
    {
        {"pathToPlayground", pathToPlayground},
        {"Compile",
            {
                {"compileArgs", compileArgs},
                {"outputBinaryFileName", outputBinaryFileName},
                {"compileStdErrFileName", compileStdErrFileName}
            }
        }
    };
}

void Config::parse(const json &js)
{
    pathToPlayground = js["pathToPlayground"];

    compileArgs = js["Compile"]["compileArgs"];
    outputBinaryFileName = js["Compile"]["outputBinaryFileName"];
    compileStdErrFileName = js["Compile"]["compileStdErrFileName"];
}