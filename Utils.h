#pragma once

#include <fstream>

namespace Utils
{
    size_t GetCharCount(std::ifstream& ifs, char ch);
    size_t GetLineCount(std::ifstream& ifs);

    size_t NumberLength(double number);

    void lstrip(std::string& str, char toBeRemoved);
    void rstrip(std::string& str, char toBeRemoved);
    void strip(std::string& str, char toBeRemoved);
    void split(const std::string& str, char delimiter, std::string& left, std::string& right);
};