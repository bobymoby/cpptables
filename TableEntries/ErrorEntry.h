#pragma once

#include "TableEntry.h"


class ErrorEntry : public TableEntry
{
private:

    MyString errorMsg;

public:

    ErrorEntry(const MyString& inputValue, const MyString& errorMsg);

    const MyString& getErrorMsg() const;

    EntryType getType() const override;
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    TableEntry* clone() const override;

};