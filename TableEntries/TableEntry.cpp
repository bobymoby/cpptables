#include "TableEntry.h"


TableEntry::TableEntry(const MyString& inputValue)
{
    this->inputValue = inputValue;
}

const MyString& TableEntry::getInputValue() const
{
    return inputValue;
}

unsigned int TableEntry::getOutputWidth() const
{
    return inputValue.size();
}