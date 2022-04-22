#pragma once
#include "headers.hpp"

#define X_MAKE_STR(x) #x
#define MAKE_STR(x) X_MAKE_STR(x)
#define Reflective(type, name, flags) reflective<type> name { MAKE_STR(name), flags }
#define ReflectiveV(type, name, value, flags) reflective<type> name { MAKE_STR(name), value, flags }
#define StringRef(name, value, flags) ReflectiveV(std::string, name, value, flags)

typedef unsigned int Flags;

template<typename T>
struct reflective
{
    string key;
    T value;

    Flags flags;

    reflective (const string& key, const T& value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    reflective (const string& key, Flags flags) {
        this->key = key;
        this->flags = flags;
    }

    inline operator T&() { return value; }
    inline operator const T&() const { return value; }

    inline reflective& operator= (const T& str) { value = str; return *this; }
};
