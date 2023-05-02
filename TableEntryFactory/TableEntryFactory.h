#pragma once

#include "../TableEntries/TableEntry.h"

class TableEntryFactory
{
private:
    static bool isString(const std::string& inputValue);
    static bool isInteger(const std::string& inputValue);
    static bool isFloat(const std::string& inputValue);
    static bool isCommand(const std::string& inputValue);
    static bool isTypeNull(const std::string& inputValue);

public:
    static TableEntry* createEntry(const std::string& inputValue);

};