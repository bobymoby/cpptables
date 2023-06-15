#include "ErrorEntry.h"


ErrorEntry::ErrorEntry(const MyString& value, const MyString& errorMsg) : TableEntry(value)
{
    this->errorMsg = errorMsg;
}

const MyString& ErrorEntry::getErrorMsg() const
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