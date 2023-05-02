#pragma once
#include <string>

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
    EntryType type;
    std::string inputValue;
    unsigned int outputWidth;
    unsigned int numberWidth;

    TableEntry(EntryType type, const std::string& inputValue);

public:
    EntryType getType() const;
    unsigned int getOutputWidth() const;
    const std::string& getInputValue() const;
    virtual double getNumberValue() const;
    virtual unsigned int getNumberWidth() const;
};