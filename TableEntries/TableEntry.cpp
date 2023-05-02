#include "TableEntry.h"

TableEntry::TableEntry(EntryType type, const std::string& inputValue):
    type(type), inputValue(inputValue), outputWidth(0), numberWidth(1)
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

unsigned int TableEntry::getNumberWidth() const
{
    return 1;
}