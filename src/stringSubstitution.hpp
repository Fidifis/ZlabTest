#pragma once
#include "headers.hpp"

constexpr char START_TOKEN[] = "$(";
constexpr char END_TOKEN[] = ")";

inline bool findPosition(const string &text, size_t &startPos, size_t &length)
{
    startPos = text.find(START_TOKEN, 0);
    if (startPos != string::npos)
    {
        length = text.find(END_TOKEN, startPos) - startPos + 1;
        if (length != string::npos)
            return true;
    }
    return false;
}

inline bool getSubstVarName(const string &text, string &name, size_t &startPosition, size_t &length)
{
    if (findPosition(text, startPosition, length))
    {
        name = text.substr(startPosition + 2, length - 3);
        return true;
    }
    else return false;
}

inline void substitute(string &text, const string &substitution, const size_t startPosition, const size_t length)
{
    text.replace(startPosition, length, substitution);
}
