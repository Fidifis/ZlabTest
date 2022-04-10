#include "difference.hpp"

void difference(const TaskManager *taskm)
{
    for (size_t i = 0; i < taskm->size(); ++i)
    {
        const TaskTest *test = (*taskm)[i];

        const string &outputData = test->getOutputData();
        const string &referenceData = test->getReferenceData();
        if (!filesystem::exists(outputData))
        {
            throw invalid_argument("Path does not exists: " + outputData);
        }
        if (!filesystem::is_directory(outputData))
        {
            throw invalid_argument("Path is not directory: " + outputData);
        }
        if (!filesystem::exists(referenceData))
        {
            throw invalid_argument("Path does not exists: " + referenceData);
        }
        if (!filesystem::is_directory(referenceData))
        {
            throw invalid_argument("Path is not directory: " + referenceData);
        }

        cout << "Comparing with reference from: " << referenceData << endl;

        vector<string> diffTests;
        for (const auto &file : filesystem::directory_iterator(outputData)) {
            if (!file.exists() || file.is_directory())
                continue;

            const string filename = file.path().filename().string();

            cout << test->getTaskName() << ": " << test->getTestName() << ": " << filename << endl;

            const string cmd = "bash '" + Scripts::getDifference() + "' '" +
                outputData + filename + "' '" +
                referenceData + filename + "' '" +
                test->getDifferenceData() + filename + "'";
            int exitCode = system(cmd.c_str());

            if (exitCode != 0)
            {
                diffTests.push_back(filename);
            }
        }

        test->result->differingOutput = diffTests;
    }
}
