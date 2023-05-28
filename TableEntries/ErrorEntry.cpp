#include "ErrorEntry.h"

ErrorEntry::ErrorEntry(const std::string& value, const std::string& errorMsg) : TableEntry(value)
{
    this->errorMsg = errorMsg;
}

const std::string& ErrorEntry::getErrorMsg() const
{
    return errorMsg;
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
