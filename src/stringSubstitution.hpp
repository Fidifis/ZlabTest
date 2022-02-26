#pragma once
#include "headers.hpp"

const char __startToken[] = "$(";
const char __endToken[] = ")";

inline bool __findPosition(const string &text, size_t &startPos, size_t &endPos);

inline bool getSubstVarName(const string &text, string &name);
inline bool getSubstVarName(const string &text, string &name, size_t &startPosition, size_t &length);

inline void substitute(string &text, const string &substitution);
inline void substitute(string &text, const string &substitution, const size_t startPosition, const size_t length);
