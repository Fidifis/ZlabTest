#pragma once
#include "headers.hpp"

using nlohmann::json;

class Config
{
public:
    string pathToPlayground = "/home/zlabtest/playground/";
    //compile
    string outputBinaryFileName = "out.bin";
    string compileStdErrFileName = "compile_stderr";
    //run
    string outputDataPath = pathToPlayground + "output/";

    void load(const char path[]);

private:
    void toJSON(json &js);
    void parse(const json &js);
};