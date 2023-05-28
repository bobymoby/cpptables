#pragma once

#include "TableEntry.h"

class TypeNullEntry : public TableEntry
{
public:
    TypeNullEntry();

    EntryType getType() const override;
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;

    TableEntry* clone() const override;
};