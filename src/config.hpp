#pragma once
#include "headers.hpp"

using nlohmann::json;

class Config
{
public:
    string pathToPlayground = "/home/filipus/Plocha/playground/";
    string compileArgs = "-Wall -pedantic";
    string outputBinaryFileName = "out.bin";
    string compileStdErrFileName = "compile_stderr";

    void load(const char path[]);

private:
    void toJSON(json &js);
    void parse(const json &js);
};