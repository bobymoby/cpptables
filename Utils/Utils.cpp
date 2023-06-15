#include "Utils.hpp"

#include <string>
#include <iostream>
#include <iomanip> // std::setprecision std::fixed std::defaultfloat


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

bool Utils::isInt(double number)
{
    return number == (int)number;
}

bool Utils::isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool Utils::isInt(const MyString& str)
{
    bool hasSign = str.front() == '+' || str.front() == '-';
    for (int i = hasSign; i < str.size(); i++)
    {
        if (!isDigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Utils::isFloat(const MyString& str)
{
    bool hasSign = str.front() == '+' || str.front() == '-';
    bool hasDot = false;
    for (int i = hasSign; i < str.size(); i++)
    {
        if (str[i] == '.')
        {
            if (hasDot)
            {
                return false;
            }
            hasDot = true;
        }
        else if (!isDigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

size_t Utils::numlen(long long number)
{
    if (number == 0)
    {
        return 1;
    }
    size_t length = 0;
    if (number < 0)
    {
        length++;
    }
    while (number != 0)
    {
        number /= 10;
        length++;
    }
    return length;
}

MyString Utils::itoa(long long number)
{
    MyString result;
    if (number == 0)
    {
        result += '0';
        return result;
    }
    size_t index = 0;
    bool isNegative = false;
    if (number < 0)
    {
        result += '-';
        number *= -1;
        isNegative = true;
    }
    while (number > 0)
    {
        result += number % 10 + '0';
        number /= 10;
    }
    if (isNegative)
    {
        result += '-';
        index++;
    }
    reverse(result);
    return result;
}

void Utils::itoa(long long number, char* buffer)
{
    if (number == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    size_t index = 0;
    bool isNegative = false;
    if (number < 0)
    {
        buffer[index] = '-';
        number *= -1;
        isNegative = true;
    }
    while (number > 0)
    {
        buffer[index] = number % 10 + '0';
        number /= 10;
        index++;
    }
    if (isNegative)
    {
        buffer[index] = '-';
        index++;
    }
    buffer[index] = '\0';
    reverse(buffer);
}

long long Utils::stoi(const MyString& str)
{
    long long result = 0;
    size_t index = 0;
    bool isNegative = false;
    if (str[0] == '-')
    {
        isNegative = true;
        index++;
    }
    while (str[index] != '\0')
    {
        result *= 10;
        result += str[index] - '0';
        index++;
    }
    if (isNegative)
    {
        result *= -1;
    }
    return result;
}

double Utils::stod(const MyString& str)
{
    if (!isFloat(str))
    {
        MyString msg = str + " is not a valid float";
        throw std::invalid_argument(msg.c_str());
    }
    double result = 0;
    size_t index = 0;
    bool isNegative = false;
    if (str[0] == '-')
    {
        isNegative = true;
        index++;
    }
    while (str[index] != '\0' && str[index] != '.')
    {
        result *= 10;
        result += str[index] - '0';
        index++;
    }
    if (str[index] == '.')
    {
        index++;
        double power = 1;
        while (str[index] != '\0')
        {
            power /= 10;
            result += (str[index] - '0') * power;
            index++;
        }
    }
    if (isNegative)
    {
        result *= -1;
    }
    return result;
}

MyString Utils::dtoa(double number)
{
    MyString result = std::to_string(number);
    rstrip(result, '0');
    if (result.back() == '.')
    {
        result = result.substr(0, result.size() - 1);
    }
    return result;
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

int Utils::strcmp(const char* str1, const char* str2)
{
    size_t index = 0;
    while (str1[index] != '\0' && str2[index] != '\0')
    {
        if (str1[index] < str2[index])
        {
            return -1;
        }
        if (str1[index] > str2[index])
        {
            return 1;
        }
        index++;
    }

    if (str1[index] == '\0' && str2[index] == '\0')
    {
        return 0;
    }
    if (str1[index] == '\0')
    {
        return -1;
    }
    return 1;
}

void Utils::reverse(MyString& str)
{
    size_t length = str.size();
    for (size_t i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void Utils::reverse(char* str)
{
    size_t length = strlen(str);
    char temp;
    for (size_t i = 0; i < length / 2; i++)
    {
        temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void Utils::resetFloatPrecision()
{
    std::cout << std::setprecision(6);
    std::cout << std::defaultfloat;
}

void Utils::setFloatPrecision(size_t precision)
{
    std::cout << std::setprecision(precision);
}