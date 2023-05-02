#include "CommandEntry.h"
#include "../Utils.h"

#include <iostream>

CommandEntry::CommandEntry(const std::string& inputValue):
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
    std::string lside;
    std::string rside;
    size_t index = 1;
    size_t startIndex = 0;
    size_t endIndex = 0;
    while (index < inputValue.size() && inputValue[index] == ' ')
    {
        index++;
    }
    startIndex = index;
    while (true)
    {
        if (inputValue[index] == '+')
        {
            op = Operation::Add;
            endIndex = index;
            break;
        }
        else if (inputValue[index] == '-')
        {
            op = Operation::Subtract;
            endIndex = index;
            break;
        }
        else if (inputValue[index] == '*')
        {
            op = Operation::Multiply;
            endIndex = index;
            break;
        }
        else if (inputValue[index] == '/')
        {
            op = Operation::Divide;
            endIndex = index;
            break;
        }
        else if (index == inputValue.size() - 1)
        {
            endIndex = index + 1;
            break;
        }
        index++;
    }
    lside = inputValue.substr(startIndex, endIndex - startIndex);
    rside = inputValue.substr(endIndex + 1);

    std::cout << lside << " " << rside << std::endl;
    std::cout << "op: " << (int)op << std::endl;

    readIndexes(lside, true);
    readIndexes(rside, false);
}

size_t CommandEntry::getLCIndex() const
{
    return leftColIndex;
}

size_t CommandEntry::getLRIndex() const
{
    return leftRowIndex;
}

size_t CommandEntry::getRCIndex() const
{
    return rightColIndex;
}

Operation CommandEntry::getOperation() const
{
    return op;
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
