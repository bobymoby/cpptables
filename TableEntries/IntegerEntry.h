#pragma once

#include "TableEntry.h"


class IntegerEntry : public TableEntry
{
public:

    IntegerEntry(const MyString& inputValue);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    EntryType getType() const override;

    TableEntry* clone() const override;

};