#pragma once
#include "headers.hpp"

#define X_MAKE_STR(x) #x
#define MAKE_STR(x) X_MAKE_STR(x)
#define Reflective(type, name, flags) stringRef<type> name { MAKE_STR(name), flags }
#define ReflectiveV(type, name, value, flags) stringRef<type> name { MAKE_STR(name), value, flags }
#define StringRef(name, value, flags) ReflectiveV(std::string, name, value, flags)

typedef unsigned int Flags;

//Reflective String
template<typename T>
struct stringRef
{
    string key;
    T value;

    Flags flags;

    stringRef (const string& key, const T& value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    stringRef (const string& key, Flags flags) {
        this->key = key;
        this->flags = flags;
    }

    inline operator T&() { return value; }
    inline operator const T&() const { return value; }

    inline stringRef& operator= (const T& str) { value = str; return *this; }
};
