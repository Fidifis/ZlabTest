#include "result.hpp"

json Result::toJson() const
{
    json js;

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

void Result::saveResult(const vector<TaskTest*> &tasks, const string &path) {
    cout << "Saving result file to: " << path << endl;
    ofstream stream(path);
    json js;

    for (const TaskTest *test : tasks)
    {
        js[test->getTestName()] = test->result->toJson();
    }

    stream << setw(4) << js;
    stream.close();
}
