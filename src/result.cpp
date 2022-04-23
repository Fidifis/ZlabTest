#include "result.hpp"

json Result::toJson() const
{
    json js;

    js["state"] = resultStateToString(state);
    js["successPercent"] = successPercent;
    js["compileResult"] = compileResultToString(compileResult);

    if (unexpectedExitCodes.size() == 0)
    {
        js["unexpectedExitCodes"] = "none";
    }
    else
    {
        for (const auto& item : unexpectedExitCodes)
        {
            string s;
            if (EXIT_CODE_MAP.count(item.second))
            {
                s = to_string(item.second) + " (" + EXIT_CODE_MAP.at(item.second) + ")";
            }
            else
            {
                s = to_string(item.second);
            }
            js["unexpectedExitCodes"][item.first] = s;
        }
    }


    if (differingOutput.size() == 0)
    {
        js["differingOutput"] = "none";
    }
    else
    {
        js["differingOutput"] = differingOutput;
    }
    return js;
}
