#pragma once

#include "TableEntry.h"
#include "FloatEntry.h"


enum class Operation
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER
};

enum class Side
{
    LEFT,
    RIGHT
};

class CommandEntry : public TableEntry
{
private:

    FloatEntry output;

    bool _executed;

    bool _shouldBeError;
    MyString errorMsg;

    Operation op;

    bool isLeftCell; // if false, then it's a number
    size_t leftColIndex;
    size_t leftRowIndex;
    double leftNumberValue;

    bool isRightCell; // if false, then it's a number
    size_t rightColIndex;
    size_t rightRowIndex;
    double rightNumberValue;

    void parseCommand();
    void readIndexes(MyString& str, Side side);

    bool isIndex(const MyString& str) const;

public:

    CommandEntry(const MyString& inputValue);
    double getNumberValue() const override;
    unsigned int getNumberWidth() const override;
    unsigned int getDecimalPlaces() const;
    EntryType getType() const override;

    void execute(double result);
    bool hasExecuted() const;
    void reset();

    const MyString& getErrorMsg() const;
    bool shouldBeError() const;

    bool getIsLeftCell() const;
    size_t getLCIndex() const;
    size_t getLRIndex() const;
    double getLNumberValue() const;
    bool getIsRightCell() const;
    size_t getRCIndex() const;
    size_t getRRIndex() const;
    double getRNumberValue() const;

    Operation getOperation() const;

    TableEntry* clone() const override;

};