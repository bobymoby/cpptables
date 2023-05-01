#include "TableEntry.h"



TableEntry::TableEntry(EntryType type, const std::string& inputValue): type(type), inputValue(inputValue)
{
}

EntryType TableEntry::getType() const
{
    return type;
}

unsigned int TableEntry::getOutputWidth() const
{
    return outputWidth;
}

const std::string& TableEntry::getInputValue() const
{
    return inputValue;
}

double TableEntry::getNumberValue() const
{
    return 0;
}

StringEntry::StringEntry(const std::string& inputValue): TableEntry(EntryType::STRING, inputValue)
{
    outputWidth = inputValue.size();
}

IntegerEntry::IntegerEntry(const std::string& inputValue): TableEntry(EntryType::INTEGER, inputValue)
{
    outputWidth = inputValue.size();
}

double IntegerEntry::getNumberValue() const
{
    return std::stod(inputValue);
}

FloatEntry::FloatEntry(const std::string& inputValue): TableEntry(EntryType::FLOAT, inputValue)
{
    outputWidth = inputValue.size();
}

double FloatEntry::getNumberValue() const
{
    return std::stod(inputValue);
}

CommandEntry::CommandEntry(const std::string& inputValue): TableEntry(EntryType::COMMAND, inputValue)
{
}

void CommandEntry::execute(double val1, double val2)
{
    //wip
}

double CommandEntry::getNumberValue() const
{
    return numberValue;
}

TypeNullEntry::TypeNullEntry(const std::string& value): TableEntry(EntryType::TYPENULL, inputValue)
{
    outputWidth = 0;
}

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
        return new CommandEntry(value.substr(1));
    }
    return new ErrorEntry(value);
}

ErrorEntry::ErrorEntry(const std::string& value): TableEntry(EntryType::ERROR, inputValue)
{
    outputWidth = value.size();
}
