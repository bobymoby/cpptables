#pragma once

#include <fstream>
#include <vector>

namespace Utils
{
    size_t GetCharCount(std::ifstream& ifs, char ch);
    size_t GetLineCount(std::ifstream& ifs);

    size_t NumberLength(double number);

    void lstrip(std::string& str, char toBeRemoved);
    void rstrip(std::string& str, char toBeRemoved);
    void strip(std::string& str, char toBeRemoved);
    void split(const std::string& str, char delimiter, std::string& left, std::string& right);

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