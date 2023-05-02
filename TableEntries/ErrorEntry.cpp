#include "ErrorEntry.h"

ErrorEntry::ErrorEntry(const std::string& value): TableEntry(EntryType::ERROR, "ERROR: " + value)
{
    outputWidth = value.size() + 7;
}