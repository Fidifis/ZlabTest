#include "result.hpp"

void Result::save(const char *path) const
{
    json js;
    ofstream outstream(path);

    js["compileResult"] = compileResultToString(compileResult);
    for (const auto& item : runExitCodes)
    {
        js["runExitCodes"][item.first] =
        item.second + " (" + EXIT_CODE_MAP.at(item.second) + ")";
    }

    outstream << js;
    outstream.close();
}
