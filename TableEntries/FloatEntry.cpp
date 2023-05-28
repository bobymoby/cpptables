#include "FloatEntry.h"

FloatEntry::FloatEntry(const std::string& inputValue) : TableEntry(inputValue)
{
    size_t decimalPointIndex = inputValue.find('.');
    if (decimalPointIndex != std::string::npos)
    {
        decimalPlaces = getOutputWidth() - decimalPointIndex - 1;
    }
    else
    {
        decimalPlaces = 0;
    }
}

double FloatEntry::getNumberValue() const
{
    return std::stod(inputValue);
}

unsigned int FloatEntry::getNumberWidth() const
{
    return inputValue.size();
}

unsigned int FloatEntry::getDecimalPlaces() const
{
    return decimalPlaces;
}

EntryType FloatEntry::getType() const
{
    return EntryType::FLOAT;
}

TableEntry* FloatEntry::clone() const
{
    return new FloatEntry(*this);
}
