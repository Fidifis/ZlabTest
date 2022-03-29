#pragma once
#include "headers.hpp"

typedef int ExitCode;

#define MAX_EXPECTED_CODE 3

const map<int, string> EXIT_CODE_MAP = {
    {0, "OK"},
    {1, "minor_problem"},
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
