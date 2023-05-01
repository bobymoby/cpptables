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

    TableEntry(EntryType type, const std::string& inputValue);

public:
    EntryType getType() const;
    unsigned int getOutputWidth() const;
    const std::string& getInputValue() const;
    virtual double getNumberValue() const;
};

class StringEntry: public TableEntry
{
public:
    StringEntry(const std::string& inputValue);
};

class IntegerEntry: public TableEntry
{
public:
    IntegerEntry(const std::string& inputValue);
    double getNumberValue() const override;
};

class FloatEntry: public TableEntry
{
public:
    FloatEntry(const std::string& inputValue);
    double getNumberValue() const override;
};

class CommandEntry: public TableEntry
{
private:
    double numberValue;
    void execute(double val1, double val2);
public:
    CommandEntry(const std::string& inputValue);
    double getNumberValue() const override;
};

class TypeNullEntry: public TableEntry
{
public:
    TypeNullEntry(const std::string& inputValue);
};

class ErrorEntry: public TableEntry
{
public:
    ErrorEntry(const std::string& inputValue);
};

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