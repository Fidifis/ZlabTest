#pragma once
#include "headers.hpp"

//#define StringRef(name, value, flags) stringRef name = stringRef(#name, value, flags)
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
        cout << "lolik1 " << key << " omg " << this->key << endl;
    }

    stringRef (const char * key, const char * value) {
        this->key = key;
        this->value = value;
    }

    stringRef (const char * key, const char * value, Flags flags) {
        this->key = key;
        this->value = value;
        this->flags = flags;
        cout << "lolik2 " << key << " omg " << this->key << endl;
    }

    //inline operator string() { return value; }
    //inline operator const string() const { return value; }

/*
    inline operator string&() { cout << "string cast " << value << endl; return value; }
    inline operator const string&() const { cout << "const string cast " << value << endl; return value; }

    inline stringRef& operator= (const string& str) { value = str; return *this; }
    inline stringRef& operator= (const char * str) { value = str; return *this; }
*/

    /*inline stringRef& operator= (const stringRef& str) {
        value = str.value;
        key = str.key;
        flags = str.flags;
    return *this; }*/
};
