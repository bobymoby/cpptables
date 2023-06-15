#pragma once

#include "../TableEntries/TableEntry.h"


class TableEntryFactory
{
private:

    static bool isString(const MyString& inputValue);
    static bool isCommand(const MyString& inputValue);
    static bool isFalseCommand(const MyString& inputValue);
    static bool isTypeNull(const MyString& inputValue);

public:

    static TableEntry* createEntry(const MyString& inputValue);

};