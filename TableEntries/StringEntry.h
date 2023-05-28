#pragma once

#include "TableEntry.h"

class StringEntry : public TableEntry
{
public:
    StringEntry(const std::string& inputValue);

    EntryType getType() const override;
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    TableEntry* clone() const override;
};