#include "FloatEntry.h"

FloatEntry::FloatEntry(const std::string& inputValue): TableEntry(EntryType::FLOAT, inputValue)
{
    outputWidth = inputValue.size();
}

double FloatEntry::getNumberValue() const
{
    return std::stod(inputValue);
}

unsigned int FloatEntry::getNumberWidth() const
{
    return inputValue.size();
}