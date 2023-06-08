#pragma once

#include <fstream>
#include <vector>

#include "MyString/MyString.h"

namespace Utils
{
    size_t GetCharCount(std::ifstream& ifs, char ch);
    size_t GetLineCount(std::ifstream& ifs);

    size_t NumberLength(double number);

    void lstrip(MyString& str, char toBeRemoved);
    void rstrip(MyString& str, char toBeRemoved);
    void strip(MyString& str, char toBeRemoved);
    void split(const MyString& str, char delimiter, MyString& left, MyString& right);

    void strcpy(char* destination, const char* source);
    void strcat(char* destination, const char* source);
    size_t strlen(const char* str);

    template <typename T>
    bool contains(const std::vector<T>& vec, const T& element);
};

template <typename T>
bool Utils::contains(const std::vector<T>& vec, const T& element)
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