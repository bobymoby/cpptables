#include "TableEntryFactory.h"
#include "../TableEntries/IntegerEntry.h"
#include "../TableEntries/FloatEntry.h"
#include "../TableEntries/CommandEntry.h"
#include "../TableEntries/StringEntry.h"
#include "../TableEntries/TypeNullEntry.h"
#include "../TableEntries/ErrorEntry.h"


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
        return new StringEntry(value.substr(1, value.size() - 2));
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
        return new CommandEntry(value);
    }
    return new ErrorEntry(value);
}