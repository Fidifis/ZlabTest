#pragma once
#include <iostream>
#include <string>

#define ANSI_RESET "\033[0m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[31m"
#define ANSI_BLUE "\033[31m"

enum class Logcol {reset, red, green, blue};

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