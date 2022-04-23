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

    unsigned long long maxTime = 0;

    for (const auto &file : filesystem::directory_iterator(inputData)) {
        if (!file.exists() || file.is_directory())
            continue;

        const string filename = file.path().filename().string();

        cout << task->getTaskName() << ": " << task->getTestName() << ": " << filename << endl;

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
            file.path().string() + "' '" +
            task->getCompiledBinaryFile() + "' '" +
            task->getOutputData() + filename + "' '" +
            task->getOutputErrors() + filename + "' '" +
            timeFile + "'";

        const int exitCode = system(cmd.c_str()) >> 8;

        if (exitCode > MAX_EXPECTED_CODE)
        {
            exitCodes[filename] = exitCode;
        }

        try
        {
            ifstream timeFileStream(timeFile);
            string content;
            getline(timeFileStream, content, '\0');
            timeFileStream.close();

            unsigned long long time = stoul(content);
            if (time > maxTime) maxTime = time;
        }
        catch(const exception& e)
        {
            cerr << Logcol::yellow <<
            "ERROR WHEN PARSING FROM TIME FILE: " << endl << e.what() <<
            Logcol::reset << endl;
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
