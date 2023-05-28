#include "TableEntryFactory.h"
#include "../TableEntries/IntegerEntry.h"
#include "../TableEntries/FloatEntry.h"
#include "../TableEntries/CommandEntry.h"
#include "../TableEntries/StringEntry.h"
#include "../TableEntries/TypeNullEntry.h"
#include "../TableEntries/ErrorEntry.h"

#include <iostream>


bool TableEntryFactory::isString(const std::string& inputValue)
{
    return inputValue.front() == '"' && inputValue.back() == '"';
}

bool TableEntryFactory::isInteger(const std::string& inputValue)
{
    bool hasSign = inputValue.front() == '+' || inputValue.front() == '-';
    for (int i = hasSign; i < inputValue.size(); i++)
    {
        if (!isdigit(inputValue[i]))
        {
            return false;
        }
    }
    return true;
}

bool TableEntryFactory::isFloat(const std::string& inputValue)
{
    bool hasSign = inputValue.front() == '+' || inputValue.front() == '-';
    bool hasDot = false;
    for (int i = hasSign; i < inputValue.size(); i++)
    {
        if (inputValue[i] == '.')
        {
            if (hasDot)
            {
                return false;
            }
            hasDot = true;
        }
        else if (!isdigit(inputValue[i]))
        {
            return false;
        }
    }
    return true;
}

bool TableEntryFactory::isCommand(const std::string& inputValue)
{
    return inputValue.front() == '=';
}

bool TableEntryFactory::isFalseCommand(const std::string& inputValue)
{
    size_t plusCount = 0;
    size_t minusCount = 0;
    size_t starCount = 0;
    size_t slashCount = 0;
    size_t powCount = 0;

    for (size_t i = 1; i < inputValue.size(); i++)
    {
        switch (inputValue[i])
        {
        case '+':
            plusCount++;
            break;
        case '-':
            minusCount++;
            break;
        case '*':
            starCount++;
            break;
        case '/':
            slashCount++;
            break;
        case '^':
            powCount++;
            break;
        }
    }

    return plusCount + minusCount + starCount + slashCount + powCount != 1;
}

bool TableEntryFactory::isTypeNull(const std::string& inputValue)
{
    return inputValue.empty();
}

TableEntry* TableEntryFactory::createEntry(const std::string& inputValue)
{
    if (isTypeNull(inputValue))
    {
        return new TypeNullEntry();
    }
    if (isString(inputValue))
    {
        std::string stripped = inputValue.substr(1, inputValue.size() - 2);
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
    if (isInteger(inputValue))
    {
        return new IntegerEntry(inputValue);
    }
    if (isFloat(inputValue))
    {
        return new FloatEntry(inputValue);
    }
    if (isCommand(inputValue))
    {
        if (isFalseCommand(inputValue))
        {
            return new ErrorEntry(inputValue, "0 or more than 1 operation signs");
        }
        return new CommandEntry(inputValue);
    }
    return new ErrorEntry(inputValue, "Unknown type");
}