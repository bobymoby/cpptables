#include "CommandEntry.h"
#include "../Utils.hpp"

#include <iostream>

CommandEntry::CommandEntry(const MyString& inputValue) : TableEntry(inputValue)
{
    numberValue = 0;
    executed = false;

    readCommand();
}

void CommandEntry::execute(double result)
{
    executed = true;
    numberValue = result;
}

double CommandEntry::getNumberValue() const
{
    return numberValue;
}

unsigned int CommandEntry::getNumberWidth() const
{
    int whole = (int)numberValue;
    int fract = (int)((numberValue - whole) * 100);
    if (fract == 0)
    {
        return MyString::fromInt(whole).size();
    }
    if (fract == 1)
    {
        return MyString::fromInt(whole).size() + 2;
    }
    return MyString::fromInt(whole).size() + 3;
}

EntryType CommandEntry::getType() const
{
    return EntryType::COMMAND;
}

void CommandEntry::readCommand()
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
        std::cout << "Invalid command: " << inputStr << std::endl;
    }
    Utils::strip(lside, ' ');
    Utils::strip(rside, ' ');

    if (lside[0] == 'R')
    {
        isLeftCell = true;
        readIndexes(lside, true);
    }
    else
    {
        isLeftCell = false;
        try
        {
            leftNumberValue = Utils::stod(lside.c_str());
        }
        catch (const std::exception&)
        {
            std::cout << "Invalid command argument: " << lside << std::endl;
        }
    }

    if (rside[0] == 'R')
    {
        isRightCell = true;
        readIndexes(rside, false);
    }
    else
    {
        isRightCell = false;
        try
        {
            rightNumberValue = Utils::stod(rside.c_str());
        }
        catch (const std::exception&)
        {
            std::cout << "Invalid command argument: " << rside << std::endl;
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
    return executed;
}

void CommandEntry::reset()
{
    executed = false;
    numberValue = 0;
}

void CommandEntry::readIndexes(MyString& str, bool isLeft)
{
    Utils::strip(str, ' ');
    size_t startIndex = 1;
    size_t index = 1;
    while (index < str.size() && isdigit(str[index]))
    {
        index++;
    }
    size_t rowIndex = Utils::stoi(str.substr(startIndex, index - startIndex).c_str());
    index++;
    size_t colIndex = Utils::stoi(str.substr(index).c_str());

    if (isLeft)
    {
        leftColIndex = colIndex;
        leftRowIndex = rowIndex;
    }
    else
    {
        rightColIndex = colIndex;
        rightRowIndex = rowIndex;
    }
}
