#pragma once

#include "TableEntry.h"

class FloatEntry : public TableEntry
{
private:
    unsigned int decimalPlaces;
public:
    FloatEntry(const std::string& inputValue);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
    unsigned int getDecimalPlaces() const;
    EntryType getType() const override;

    TableEntry* clone() const override;
};