#include "TypeNullEntry.h"

TypeNullEntry::TypeNullEntry(const std::string& value): TableEntry(EntryType::TYPENULL, inputValue)
{
    outputWidth = 0;
}