#include "TableEntryFactory.h"
#include "../TableEntries/IntegerEntry.h"
#include "../TableEntries/FloatEntry.h"
#include "../TableEntries/CommandEntry.h"
#include "../TableEntries/StringEntry.h"
#include "../TableEntries/TypeNullEntry.h"
#include "../TableEntries/ErrorEntry.h"

#include <iostream>


bool TableEntryFactory::isString(const std::string& value)
{
    return value.front() == '"' && value.back() == '"';
}

bool TableEntryFactory::isInteger(const std::string& value)
{
    bool hasSign = value.front() == '+' || value.front() == '-';
    for (int i = hasSign; i < value.size(); i++)
    {
        if (!isdigit(value[i]))
        {
            return false;
        }
    }
    return true;
}

bool TableEntryFactory::isFloat(const std::string& value)
{
    bool hasSign = value.front() == '+' || value.front() == '-';
    bool hasDot = false;
    for (int i = hasSign; i < value.size(); i++)
    {
        if (value[i] == '.')
        {
            if (hasDot)
            {
                return false;
            }
            hasDot = true;
        }
        else if (!isdigit(value[i]))
        {
            return false;
        }
    }
    return true;
}

bool TableEntryFactory::isCommand(const std::string& value)
{
    return value.front() == '=';
}

bool TableEntryFactory::isFalseCommand(const std::string& inputValue)
{
    size_t plusCount = 0;
    size_t minusCount = 0;
    size_t starCount = 0;
    size_t slashCount = 0;

    for (size_t i = 1; i < inputValue.size(); i++)
    {
        if (inputValue[i] == '+')
        {
            plusCount++;
        }
        else if (inputValue[i] == '-')
        {
            minusCount++;
        }
        else if (inputValue[i] == '*')
        {
            starCount++;
        }
        else if (inputValue[i] == '/')
        {
            slashCount++;
        }
    }

    return plusCount + minusCount + starCount + slashCount != 1;
}

bool TableEntryFactory::isTypeNull(const std::string& value)
{
    return value.empty();
}

TableEntry* TableEntryFactory::createEntry(const std::string& value)
{
    if (isTypeNull(value))
    {
        return new TypeNullEntry(value);
    }
    if (isString(value))
    {
        std::string stripped = value.substr(1, value.size() - 2);
        if (stripped.size() > 1)
        {
            for (size_t i = 0; i < stripped.size() - 1; i++)
            {
                if (stripped[i] == '\\' && stripped[i + 1] == '"')
                {
                    stripped.erase(i, 1);
                }
            }
        }

        return new StringEntry(stripped);
    }
    if (isInteger(value))
    {
        return new IntegerEntry(value);
    }
    if (isFloat(value))
    {
        return new FloatEntry(value);
    }
    if (isCommand(value))
    {
        if (isFalseCommand(value))
        {
            return new ErrorEntry(value + " has 0 or more than 1 operation signs.");
        }
        return new CommandEntry(value);
    }
    return new ErrorEntry(value);
}