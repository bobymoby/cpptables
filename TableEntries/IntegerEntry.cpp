#include "IntegerEntry.h"

IntegerEntry::IntegerEntry(const std::string& inputValue): TableEntry(EntryType::INTEGER, inputValue)
{
    outputWidth = inputValue.size();
}

double IntegerEntry::getNumberValue() const
{
    return std::stod(inputValue);
}

unsigned int IntegerEntry::getNumberWidth() const
{
    return inputValue.size();
}