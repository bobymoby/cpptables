#include "CommandEntry.h"
#include "../Utils/Utils.hpp"

#include <iostream>


CommandEntry::CommandEntry(const MyString& inputValue) : TableEntry(inputValue)
{
    _shouldBeError = false;
    errorMsg = "";
    reset();
    parseCommand();
}

void CommandEntry::execute(double result)
{
    _executed = true;
    MyString resultStr = Utils::dtoa(result);
    output = FloatEntry(resultStr);
}

double CommandEntry::getNumberValue() const
{
    if (_executed)
    {
        return output.getNumberValue();
    }
    return 0;
}

unsigned int CommandEntry::getNumberWidth() const
{
    if (_executed)
    {
        return output.getNumberWidth();
    }
    return 1;
}

unsigned int CommandEntry::getDecimalPlaces() const
{
    if (_executed)
    {
        return output.getDecimalPlaces();
    }
    return 0;
}

EntryType CommandEntry::getType() const
{
    return EntryType::COMMAND;
}

void CommandEntry::parseCommand()
{
    MyString inputStr = inputValue.substr(1);

    MyString lside;
    MyString rside;

    if (inputStr.find('+') != MyString::npos)
    {
        op = Operation::ADD;
        Utils::split(inputStr, '+', lside, rside);
    }
    else if (inputStr.find('-') != MyString::npos)
    {
        op = Operation::SUBTRACT;
        Utils::split(inputStr, '-', lside, rside);
    }
    else if (inputStr.find('*') != MyString::npos)
    {
        op = Operation::MULTIPLY;
        Utils::split(inputStr, '*', lside, rside);
    }
    else if (inputStr.find('/') != MyString::npos)
    {
        op = Operation::DIVIDE;
        Utils::split(inputStr, '/', lside, rside);
    }
    else if (inputStr.find('^') != MyString::npos)
    {
        op = Operation::POWER;
        Utils::split(inputStr, '^', lside, rside);
    }
    else
    {
        _shouldBeError = true;
        errorMsg = MyString("Invalid command: ") + inputStr;
    }
    Utils::strip(lside, ' ');
    Utils::strip(rside, ' ');

    if (lside[0] == 'R')
    {
        isLeftCell = true;
        readIndexes(lside, Side::LEFT);
    }
    else
    {
        isLeftCell = false;
        try
        {
            leftNumberValue = Utils::stod(lside);
        }
        catch (const std::exception&)
        {
            _shouldBeError = true;
            errorMsg = MyString("Invalid command argument: ") + lside;
        }
    }

    if (rside[0] == 'R')
    {
        isRightCell = true;
        readIndexes(rside, Side::RIGHT);
    }
    else
    {
        isRightCell = false;
        try
        {
            rightNumberValue = Utils::stod(rside);
        }
        catch (const std::exception&)
        {
            _shouldBeError = true;
            errorMsg = MyString("Invalid command argument: ") + rside;
        }
    }
}

size_t CommandEntry::getLCIndex() const
{
    return leftColIndex;
}

size_t CommandEntry::getLRIndex() const
{
    return leftRowIndex;
}

bool CommandEntry::getIsLeftCell() const
{
    return isLeftCell;
}

size_t CommandEntry::getRCIndex() const
{
    return rightColIndex;
}

double CommandEntry::getLNumberValue() const
{
    return leftNumberValue;
}

bool CommandEntry::getIsRightCell() const
{
    return isRightCell;
}

Operation CommandEntry::getOperation() const
{
    return op;
}

TableEntry* CommandEntry::clone() const
{
    return new CommandEntry(*this);
}

double CommandEntry::getRNumberValue() const
{
    return rightNumberValue;
}

size_t CommandEntry::getRRIndex() const
{
    return rightRowIndex;
}

bool CommandEntry::hasExecuted() const
{
    return _executed;
}

void CommandEntry::reset()
{
    _executed = false;
    output = FloatEntry();
}

const MyString& CommandEntry::getErrorMsg() const
{
    return errorMsg;
}

bool CommandEntry::shouldBeError() const
{
    return _shouldBeError;
}

void CommandEntry::readIndexes(MyString& str, Side side)
{
    Utils::strip(str, ' ');
    size_t startIndex = 1;
    size_t index = 1;
    while (index < str.size() && Utils::isDigit(str[index]))
    {
        index++;
    }

    MyString rowIndexStr = str.substr(startIndex, index - startIndex);
    if (!isIndex(rowIndexStr))
    {
        _shouldBeError = true;
        errorMsg = MyString("Invalid row index: ") + rowIndexStr;
        return;
    }
    size_t rowIndex = Utils::stoi(rowIndexStr.c_str());

    MyString colIndexStr = str.substr(index + 1);
    if (!isIndex(colIndexStr))
    {
        _shouldBeError = true;
        errorMsg = MyString("Invalid column index: ") + colIndexStr;
        return;
    }
    size_t colIndex = Utils::stoi(colIndexStr.c_str());

    if (side == Side::LEFT)
    {
        leftColIndex = colIndex - 1;
        leftRowIndex = rowIndex - 1;
    }
    else
    {
        rightColIndex = colIndex - 1;
        rightRowIndex = rowIndex - 1;
    }
}

bool CommandEntry::isIndex(const MyString& str) const
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!Utils::isDigit(str[i]))
        {
            return false;
        }
    }
    return true;
}