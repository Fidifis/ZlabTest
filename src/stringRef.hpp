#pragma once
#include "headers.hpp"

//#define StringRef(name, value) stringRef name = stringRef(#name, value)
//#define StringRef(name, value) stringRef name { #name, value }
#define StringRef(name, value, flags) stringRef name { #name, value, flags }

typedef unsigned int Flags;

//Reflective String
struct stringRef
{
    string key;
    string value;

    Flags flags;

    stringRef (const string& key, const string& value) {
        this->key = key;
        this->value = value;
    }

    stringRef (const string& key, const string& value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    stringRef (const char * key, const char * value) {
        this->key = key;
        this->value = value;
    }

    stringRef (const char * key, const char * value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
    }

    //operator string() { return value; }
    operator string&() { return value; }
    //operator const string() const { return value; }
    operator const string&() const { return value; }

    stringRef& operator= (string str) { value = str; return *this; }
    stringRef& operator= (const char * str) { value = str; return *this; }
};
