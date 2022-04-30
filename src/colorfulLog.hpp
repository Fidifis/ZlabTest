#pragma once
#include <string>
#include <iostream>

const char ANSI_RESET[] = "\033[0m";
const char ANSI_RED[] = "\033[31m";
const char ANSI_GREEN[] = "\033[32m";
const char ANSI_YELLOW[] = "\033[33m";
const char ANSI_BLUE[] = "\033[34m";

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
