#include "IntegerEntry.h"
#include "../Utils/Utils.hpp"


IntegerEntry::IntegerEntry(const MyString& inputValue) : TableEntry(inputValue)
{
}

double IntegerEntry::getNumberValue() const
{
    return Utils::stod(inputValue);
}

unsigned int IntegerEntry::getNumberWidth() const
{
    return inputValue.size();
}

EntryType IntegerEntry::getType() const
{
    return EntryType::INTEGER;
}

TableEntry* IntegerEntry::clone() const
{
    return new IntegerEntry(*this);
}