#include "CommandEntry.h"
#include "../Utils.h"

#include <iostream>

CommandEntry::CommandEntry(const std::string& inputValue) :
    TableEntry(EntryType::COMMAND, inputValue), executed(false)
{
    numberValue = 0;
    outputWidth = inputValue.size();

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
        return std::to_string(whole).size();
    }
    if (fract == 1)
    {
        return std::to_string(whole).size() + 2;
    }
    return std::to_string(whole).size() + 3;
}

void CommandEntry::readCommand()
{
    std::string inputStr = inputValue.substr(1);

    std::string lside;
    std::string rside;

    if (inputStr.find('+') != std::string::npos)
    {
        op = Operation::ADD;
        Utils::split(inputStr, '+', lside, rside);
    }
    else if (inputStr.find('-') != std::string::npos)
    {
        op = Operation::SUBTRACT;
        Utils::split(inputStr, '-', lside, rside);
    }
    else if (inputStr.find('*') != std::string::npos)
    {
        op = Operation::MULTIPLY;
        Utils::split(inputStr, '*', lside, rside);
    }
    else if (inputStr.find('/') != std::string::npos)
    {
        op = Operation::DIVIDE;
        Utils::split(inputStr, '/', lside, rside);
    }
    else if (inputStr.find('^') != std::string::npos)
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
            leftNumberValue = std::stod(lside);
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
            rightNumberValue = std::stod(rside);
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

void CommandEntry::readIndexes(std::string& str, bool isLeft)
{
    Utils::strip(str, ' ');
    size_t startIndex = 1;
    size_t index = 1;
    while (index < str.size() && isdigit(str[index]))
    {
        index++;
    }
    size_t rowIndex = std::stoi(str.substr(startIndex, index - startIndex));
    index++;
    size_t colIndex = std::stoi(str.substr(index));

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
