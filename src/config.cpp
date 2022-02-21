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
        {"compile",
            {
                {"compileArgs", compileArgs},
                {"outputBinaryFileName", outputBinaryFileName},
                {"compileStdErrFileName", compileStdErrFileName}
            }
        },
        {"run",
            {
                {"outputDataPath", outputDataPath},
                {"defaultTimeout", defaultTimeout}
            }
        }
    };
}

void Config::parse(const json &js)
{
    pathToPlayground = js["pathToPlayground"];

    compileArgs = js["compile"]["compileArgs"];
    outputBinaryFileName = js["compile"]["outputBinaryFileName"];
    compileStdErrFileName = js["compile"]["compileStdErrFileName"];
    outputDataPath = js["run"]["outputDataPath"];
    defaultTimeout = js["run"]["defaultTimeout"];
}