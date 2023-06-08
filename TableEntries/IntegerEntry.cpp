#include "IntegerEntry.h"

IntegerEntry::IntegerEntry(const MyString& inputValue) : TableEntry(inputValue)
{
}

double IntegerEntry::getNumberValue() const
{
    return std::stod(inputValue.c_str());
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
