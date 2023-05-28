#pragma once

#include "TableEntry.h"

class ErrorEntry : public TableEntry
{
public:
    ErrorEntry(const std::string& inputValue);
    EntryType getType() const override;
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    TableEntry* clone() const override;
};