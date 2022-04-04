#pragma once
#include "headers.hpp"

typedef int ExitCode;

#define MAX_EXPECTED_CODE 3

const map<const ExitCode, const string> EXIT_CODE_MAP {
    {0, "OK"},
    {1, "Error"},
    {2, "serious_problem"},
    {132, "SIGILL"},
    {133, "SIGTRAP"},
    {134, "SIGABRT"},
    {135, "SIGEMT"},
    {136, "SIGFPE"},
    {137, "SIGKILL"},
    {138, "SIGBUS"},
    {139, "SIGSEGV"},
    {143, "SIGTERM"}
};
