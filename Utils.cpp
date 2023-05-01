#include "Utils.h"
#include <string>

size_t Utils::GetCharCount(std::ifstream& ifs, char ch)
{
    size_t currentPosition = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    if (!ifs.is_open())
        return 0;

    unsigned int count = 0;

    while (true)
    {
        char current = ifs.get();
        if (ifs.eof())
            break;

        if (current == ch)
            count++;
    }

    ifs.clear();
    ifs.seekg(currentPosition);
    return count;
}

size_t Utils::GetLineCount(std::ifstream& ifs)
{
    if (!ifs.is_open())
        return 0;

    return GetCharCount(ifs, '\n') + 1;
}

size_t Utils::NumberLength(double number)
{
    return 10;
}

void Utils::lstrip(std::string& str, char toBeRemoved)
{
    size_t index = 0;
    while (index < str.size() && str[index] == toBeRemoved)
    {
        index++;
    }

    str = str.substr(index);
}

void Utils::rstrip(std::string& str, char toBeRemoved)
{
    size_t index = str.size() - 1;
    while (index >= 0 && str[index] == toBeRemoved)
    {
        index--;
    }

    str = str.substr(0, index + 1);
}

void Utils::strip(std::string& str, char toBeRemoved)
{
    lstrip(str, toBeRemoved);
    rstrip(str, toBeRemoved);
}
