#include "TableEntry.h"

TableEntry::TableEntry(const std::string& inputValue)
{
    this->inputValue = inputValue;
}

const std::string& TableEntry::getInputValue() const
{
    return inputValue;
}

unsigned int TableEntry::getOutputWidth() const
{
    return inputValue.size();
}
