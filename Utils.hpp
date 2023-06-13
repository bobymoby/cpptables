#pragma once

#include <fstream>

#include "MyString/MyString.h"
#include "MyVector/MyVector.hpp"

namespace Utils
{
    size_t GetCharCount(std::ifstream& ifs, char ch);
    size_t GetLineCount(std::ifstream& ifs);

    bool isInt(double number);

    size_t numlen(long long number);
    void itoa(long long number, char* buffer);
    long long stoi(const char* str);
    double stod(const char* str);
    MyString dtoa(double number);

    void lstrip(MyString& str, char toBeRemoved);
    void rstrip(MyString& str, char toBeRemoved);
    void strip(MyString& str, char toBeRemoved);
    void split(const MyString& str, char delimiter, MyString& left, MyString& right);

    void strcpy(char* destination, const char* source);
    void strcat(char* destination, const char* source);
    size_t strlen(const char* str);
    int strcmp(const char* str1, const char* str2);

    void reverse(char* str);

    template <typename T>
    bool contains(const MyVector<T>& vec, const T& element);

    void setFloatPrecision(size_t precision);
    void resetFloatPrecision();
};

template <typename T>
bool Utils::contains(const MyVector<T>& vec, const T& element)
{
    for (const T& item : vec)
    {
        if (item == element)
        {
            return true;
        }
    }
    return false;
}