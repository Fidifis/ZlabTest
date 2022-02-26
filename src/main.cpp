#include "headers.hpp"
#include "task.hpp"
#include "compile.hpp"

const char CONFIG_FILE_NAME[] = "config.json";

void clearFiles(const Task *task)
{
    for (auto &file : filesystem::directory_iterator(task->getPlayground())) 
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

    //clearFiles(compParams);
}
