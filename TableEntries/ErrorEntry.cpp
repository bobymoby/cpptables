#include "ErrorEntry.h"

ErrorEntry::ErrorEntry(const std::string& value) : TableEntry("ERROR: " + value)
{
}

EntryType ErrorEntry::getType() const
{
    return EntryType::ERROR;
}

double ErrorEntry::getNumberValue() const
{
    return 0;
}

unsigned int ErrorEntry::getNumberWidth() const
{
    return 1;
}

TableEntry* ErrorEntry::clone() const
{
    return new ErrorEntry(*this);
}
