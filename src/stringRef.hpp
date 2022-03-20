#pragma once
#include "headers.hpp"

#define StringRef(name, value) stringRef name = stringRef(#name, value)

//Reflective String
struct stringRef
{
    string key;
    string value;

    stringRef (string key, string value) {
        this->key = key;
        this->value = value;
    }

    stringRef (const char * key, const char * value) {
        this->key = key;
        this->value = value;
    }

    operator string() { return value; }
    operator string&() { return value; }
    operator const string() const { return value; }
    operator const string&() const { return value; }

    stringRef& operator= (string str) { value = str; return *this; }
    stringRef& operator= (const char * str) { value = str; return *this; }
};
