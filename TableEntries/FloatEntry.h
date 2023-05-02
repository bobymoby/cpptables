#pragma once

#include "TableEntry.h"

class FloatEntry: public TableEntry
{
public:
    FloatEntry(const std::string& inputValue);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
};