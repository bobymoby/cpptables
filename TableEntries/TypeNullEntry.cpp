#include "TypeNullEntry.h"

TypeNullEntry::TypeNullEntry() : TableEntry("")
{
}

EntryType TypeNullEntry::getType() const
{
    return EntryType::TYPENULL;
}

double TypeNullEntry::getNumberValue() const
{
    return 0;
}

unsigned int TypeNullEntry::getNumberWidth() const
{
    return 1;
}

TableEntry* TypeNullEntry::clone() const
{
    return new TypeNullEntry();
}
