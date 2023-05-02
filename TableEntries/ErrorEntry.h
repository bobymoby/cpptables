#pragma once

#include "TableEntry.h"

class ErrorEntry: public TableEntry
{
public:
    ErrorEntry(const std::string& inputValue);
};