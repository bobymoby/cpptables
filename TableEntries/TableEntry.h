#pragma once
#include <string>
#include "../MyString/MyString.h"

enum class EntryType
{
    STRING,
    INTEGER,
    FLOAT,
    COMMAND,
    TYPENULL,
    ERROR
};

class TableEntry
{
protected:
    MyString inputValue;

    TableEntry(const MyString& inputValue);

public:
    const MyString& getInputValue() const;

    virtual EntryType getType() const = 0;
    unsigned int getOutputWidth() const;
    virtual double getNumberValue() const = 0;
    virtual unsigned int getNumberWidth() const = 0;

    virtual TableEntry* clone() const = 0;

    virtual ~TableEntry() = default;
};