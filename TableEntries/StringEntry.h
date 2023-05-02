#pragma once

#include "TableEntry.h"

class StringEntry: public TableEntry
{
public:
    StringEntry(const std::string& inputValue);
};