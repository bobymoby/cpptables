#pragma once

#include "TableEntry.h"

class IntegerEntry: public TableEntry
{
public:
    IntegerEntry(const std::string& inputValue);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
};