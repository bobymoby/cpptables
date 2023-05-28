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
    std::string inputValue;

    TableEntry(const std::string& inputValue);

public:
    const std::string& getInputValue() const;

    virtual EntryType getType() const = 0;
    unsigned int getOutputWidth() const;
    virtual double getNumberValue() const = 0;
    virtual unsigned int getNumberWidth() const = 0;

    virtual TableEntry* clone() const = 0;

    virtual ~TableEntry() = default;
};