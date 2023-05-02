#pragma once

#include "TableEntry.h"

enum class Operation
{
    Add,
    Subtract,
    Multiply,
    Divide
};

class CommandEntry: public TableEntry
{
private:
    double numberValue;

    bool executed;
    Operation op;
    size_t leftColIndex;
    size_t leftRowIndex;
    size_t rightColIndex;
    size_t rightRowIndex;

    void readCommand();
    void readIndexes(std::string& str, bool isLeft);
public:
    CommandEntry(const std::string& inputValue);
    void execute(double result);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
    bool hasExecuted() const;

    size_t getLCIndex() const;
    size_t getLRIndex() const;
    size_t getRCIndex() const;
    size_t getRRIndex() const;

    Operation getOperation() const;
};