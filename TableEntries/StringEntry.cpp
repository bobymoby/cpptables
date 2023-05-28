#include "StringEntry.h"

StringEntry::StringEntry(const std::string& inputValue) : TableEntry(inputValue)
{
}

EntryType StringEntry::getType() const
{
    return EntryType::STRING;
}

double StringEntry::getNumberValue() const
{
    return 0;
}

unsigned int StringEntry::getNumberWidth() const
{
    return 1;
}

TableEntry* StringEntry::clone() const
{
    return new StringEntry(*this);
}
