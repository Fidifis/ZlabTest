#include "headers.hpp"
#include "config.hpp"
#include "compile.hpp"

const char CONFIG_FILE_NAME[] = "config.json";

void clearFiles(const Config *params)
{
    for (auto &file : filesystem::directory_iterator(params->pathToPlayground)) 
        filesystem::remove_all(file.path());
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "No arguments. Terminating." << endl;
        return 1;
    }
    else if (argc > 2)
    {
        cerr << "Warning, too many arguments. Ignoring them." << endl;
    }

    Config *globalConf = new Config;
    globalConf->load(CONFIG_FILE_NAME);

    string sourceCodePath = argv[1];
    if (compile(sourceCodePath, globalConf)) {
        return 1;
    }

    //clearFiles(compParams);
}
