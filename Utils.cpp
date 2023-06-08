#include "Utils.hpp"
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

void Utils::lstrip(MyString& str, char toBeRemoved)
{
    size_t index = 0;
    while (index < str.size() && str[index] == toBeRemoved)
    {
        index++;
    }

    str = str.substr(index);
}

void Utils::rstrip(MyString& str, char toBeRemoved)
{
    size_t index = 0;
    while (index < str.size() && str[str.size() - index - 1] == toBeRemoved)
    {
        index++;
    }

    str = str.substr(0, str.size() - index);
}

void Utils::strip(MyString& str, char toBeRemoved)
{
    lstrip(str, toBeRemoved);
    rstrip(str, toBeRemoved);
}

void Utils::split(const MyString& str, char delimiter, MyString& left, MyString& right)
{
    size_t splitIndex = str.find(delimiter);

    if (splitIndex == MyString::npos)
    {
        left = str;
        right = "";
    }
    else
    {
        left = str.substr(0, splitIndex);
        right = str.substr(splitIndex + 1);
    }
}

void Utils::strcpy(char* destination, const char* source)
{
    size_t index = 0;
    while (source[index] != '\0')
    {
        destination[index] = source[index];
        index++;
    }
    destination[index] = '\0';
}

void Utils::strcat(char* destination, const char* source)
{
    size_t index = 0;
    while (destination[index] != '\0')
    {
        index++;
    }

    strcpy(destination + index, source);
}

size_t Utils::strlen(const char* str)
{
    size_t index = 0;
    while (str[index] != '\0')
    {
        index++;
    }
    return index;
}
