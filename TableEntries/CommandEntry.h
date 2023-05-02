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
    bool isLeftCell;
    size_t leftColIndex;
    size_t leftRowIndex;
    double leftNumberValue;
    bool isRightCell;
    size_t rightColIndex;
    size_t rightRowIndex;
    double rightNumberValue;

    void readCommand();
    void readIndexes(std::string& str, bool isLeft);
public:
    CommandEntry(const std::string& inputValue);
    void execute(double result);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
    bool hasExecuted() const;

    bool getIsLeftCell() const;
    size_t getLCIndex() const;
    size_t getLRIndex() const;
    double getLNumberValue() const;
    bool getIsRightCell() const;
    size_t getRCIndex() const;
    size_t getRRIndex() const;
    double getRNumberValue() const;

    Operation getOperation() const;
};