#pragma once
#include "headers.hpp"

#define X_MAKE_STR(x) #x
#define MAKE_STR(x) X_MAKE_STR(x)
#define StringRef(name, value, flags) stringRef name { MAKE_STR(name), value, flags }

typedef unsigned int Flags;

//Reflective String
struct stringRef
{
    string key;
    string value;

    Flags flags;

    stringRef (const string& key, const string& value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    stringRef (const char * key, const char * value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    inline operator string&() { return value; }
    inline operator const string&() const { return value; }

    inline stringRef& operator= (const string& str) { value = str; return *this; }
    inline stringRef& operator= (const char * str) { value = str; return *this; }
};
