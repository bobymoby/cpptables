#include "StringEntry.h"

StringEntry::StringEntry(const std::string& inputValue): TableEntry(EntryType::STRING, inputValue)
{
    outputWidth = inputValue.size();
}