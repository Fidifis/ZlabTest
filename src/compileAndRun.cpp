#include "compileAndRun.hpp"

inline static string getParentPath(const string &path)
{
    auto i = path.end();
    if (*i == '/') --i;
    for (; i > path.begin(); --i)
        if (*i == '/')
            return path.substr(0, i - path.begin());
    return path;
}

inline static CompileResult compile(const TaskTest *task, const char sourceCodeFile[])
{
    if (sourceCodeFile[0] == '\0')
    {
        throw invalid_argument("source code file path is empty");
    }
    if (!filesystem::exists(sourceCodeFile))
    {
        throw invalid_argument("source code file does not exists");
    }
    if (filesystem::is_directory(sourceCodeFile))
    {
        throw invalid_argument("source code file is a directory");
    }

    cout << "Compiling source code from: " << sourceCodeFile << endl;

    const string &binFile = task->getCompiledBinaryFile();
    const string &errFile = task->getCompileErrorsFile();

    if (filesystem::create_directories(getParentPath(binFile)))
        cout << "Create directory for " << binFile << endl;
    if (filesystem::create_directories(getParentPath(errFile)))
        cout << "Create directory for " << errFile << endl;

    //g++ --args-- -o '--out.bin--' '--x.cpp--' 2> '--err--'
    /*const string cmd = "g++ " + task->getCompileArgs() +
            " -o '" + binFile +
            "' '" + sourceCodeFile +
            "' 2> '" + errFile + "'";*/

    const string cmd = "bash '" + Scripts::getCompile() + "' '" +
        task->getCompileArgs() + "' '" +
        binFile + "' '"+
        sourceCodeFile + "' '" + 
        errFile + "'";

    int returnCode = system(cmd.c_str());

    if (returnCode == 0 && filesystem::exists(binFile))
    {
        if (filesystem::exists(errFile))
        {
            ifstream errfile(errFile);
            string content;
            getline(errfile, content, '\0');
            errfile.close();

            if (content.empty())
            {
                cout << "Compiled successfully." << endl;
                return CompileResult::success;
            }
            else
            {
                cout << "Compiled with warnings." << endl;
                return CompileResult::warrings;
            }
        }
        else
        {
            cout << "Compiled successfully." << endl;
            return CompileResult::success;
        }
    }
    else
    {
        cout << "Compilation failed." << endl;
        return CompileResult::fail;
    }
}

inline static void runThread(const TaskTest *task, tuple<const string, const string, unsigned long long, ExitCode> &data)
{
    const string &filename = get<0>(data);
    const string &file = get<1>(data);
    unsigned long long &time = get<2>(data);
    ExitCode &exitCode = get<3>(data);
    //cout << task->getTaskName() << ": " << task->getTestName() << ": " << filename << endl;

        //( time -f '%E' timeout --time-- cat '--file--' | '--bin--' > '--output--' 2> '--errors--' ) 2> '--timeFile--'
        /*const string cmd = "( time -f '%E' timeout " + task->getMaxTime() +
            " cat '" + file.path().string() +
            "' | '" + task->getCompiledBinaryFile() +
            "' > '" + task->getOutputData() + filename + "_out" +
            "' 2> '"+ task->getOutputErrors() + filename + "_err" +
            "' ) 2> '" + task->getOutputRunTime() + filename + "_time'";*/

        const string timeFile = task->getOutputRunTime() + filename;
        const string cmd = "bash '" + Scripts::getRunProgram() + "' '" +
            task->getMaxTime() + "' '" +
            //file.path().string() + "' '" +
            file + "' '" +
            task->getCompiledBinaryFile() + "' '" +
            task->getOutputData() + filename + "' '" +
            task->getOutputErrors() + filename + "' '" +
            timeFile + "'";

        exitCode = system(cmd.c_str()) >> 8;

        /*if (exitCode > MAX_EXPECTED_CODE)
        {
            exitCodes[filename] = exitCode;
        }*/

        try
        {
            ifstream timeFileStream(timeFile);
            string content;
            getline(timeFileStream, content, '\0');
            timeFileStream.close();

            time = stoul(content);
            //if (time > maxTime) maxTime = time;
        }
        catch(const exception& e)
        {
            cerr << Logcol::yellow <<
            "ERROR WHEN PARSING FROM TIME FILE: " << endl << e.what() <<
            Logcol::reset << endl;
        }
}

inline static unsigned long long runProgram(const TaskTest *task, map<const string, ExitCode> &exitCodes)
{
    const string &inputData = task->getInputData();
    if (!filesystem::exists(inputData))
    {
        throw invalid_argument("Path does not exists: " + inputData);
    }
    if (!filesystem::is_directory(inputData))
    {
        throw invalid_argument("Path is not directory: " + inputData);
    }

    cout << "Executing program with inputs from: " << inputData << endl;

    if (filesystem::create_directories(task->getOutputData()))
        cout << "Create directory for " << task->getOutputData() << endl;
    if (filesystem::create_directories(task->getOutputErrors()))
        cout << "Create directory for " << task->getOutputErrors() << endl;
    if (filesystem::create_directories(task->getOutputRunTime()))
        cout << "Create directory for " << task->getOutputRunTime() << endl;

    unsigned long long maxTime = 0ull;
    vector<tuple<const string, const string, unsigned long long, ExitCode>> files;

    for (const auto &file : filesystem::directory_iterator(inputData)) {
        if (!file.exists() || file.is_directory())
            continue;

        const string filename = file.path().filename().string();
        const string filePath = file.path().string();

        files.push_back(make_tuple(filename, filePath, maxTime, 0));
    }

    const int numberOfTasks = files.size();

    //TODO: přidat možnost pro konfiguraci
    const int numberOfThreads = 2;
    const int maxTimeouts = numberOfThreads;
    
    vector<tuple<thread, int>> workers(numberOfThreads);

    int activeThreads = 0;
    int i = 0;
    int timeoutedTests = 0;
    while (true)
    {
        if (activeThreads < numberOfThreads && i < numberOfTasks && timeoutedTests < maxTimeouts)
        {
            workers.push_back(make_tuple(thread(runThread, task, ref(files[i])), i));
            //cout << "deploy thread " << i << endl;
            cout << "Thread " << i << ": " << task->getTaskName() << ": " << task->getTestName() << ": " << get<0>(files[i]) << endl;
            ++activeThreads;
            ++i;
        }
        else
        {
            for (size_t j = 0; j < workers.size(); ++j)
            {
                if (get<0>(workers[j]).joinable())
                {
                    get<0>(workers[j]).join();
                    
                    const int index = get<1>(workers[j]);
                    cout << "Thread " << index << " ended" << endl;

                    if (get<3>(files[index]) > MAX_EXPECTED_CODE)
                    {
                        exitCodes[get<0>(files[index])] = get<3>(files[index]);
                        
                        if (get<3>(files[index]) == 124)
                            ++timeoutedTests;
                    }
                    if (get<2>(files[index]) > maxTime) maxTime = get<2>(files[index]);

                    workers.erase(workers.begin() + j);
                    --j;
                    --activeThreads;
                }
            }
            if (activeThreads == 0)
            {
                if (timeoutedTests >= maxTimeouts)
                {
                    cout << "Excecution stops because of " << timeoutedTests << " timeouts" << endl;
                    task->result->timeouted = true;
                    break;
                }
                else if (i >= numberOfTasks)
                    break;
            }
        }
    }

    return maxTime;
}

void compileAndRun(const TaskTest *task, bool &compiled, const char *sourceCodeFile)
{
    compiled = !task->getRecompile() && compiled;
    if (!compiled)
    {
        CompileResult res = compile(task, sourceCodeFile);
        task->result->compileResult = res;
        if (res == CompileResult::fail)
        {
            compiled = false;
            cout << "skipping " << task->getTestName() << endl;
            return;
        }
    }
    else
    {
        task->result->compileResult = CompileResult::none;
    }

    task->result->maxTime = runProgram(task, task->result->unexpectedExitCodes);
    compiled = !task->getRecompile();
}
