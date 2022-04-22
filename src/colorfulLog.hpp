#pragma once
#include "headers.hpp"

#define ANSI_RESET "\033[0m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"

enum class Logcol {reset, red, green, yellow, blue};

inline const char* getAnsiForColor(Logcol color)
{
    switch (color)
    {
    case Logcol::reset:
        return ANSI_RESET;

    case Logcol::red:
        return ANSI_RED;
    
    case Logcol::green:
        return ANSI_GREEN;

    case Logcol::yellow:
        return ANSI_YELLOW;

    case Logcol::blue:
        return ANSI_BLUE;
    }
    return "";
}

inline std::ostream& operator<<(std::ostream &stream, Logcol color)
{
    stream << getAnsiForColor(color);
    return stream;
}

inline std::string logcol(const std::string &text, Logcol color)
{
    std::string str = getAnsiForColor(color);
    str += text;
    str += getAnsiForColor(Logcol::reset);
    return str;
}
