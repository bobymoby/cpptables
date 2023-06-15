#include "Table.h"
#include "../TableEntryFactory/TableEntryFactory.h"
#include "../TableEntries/ErrorEntry.h"
#include "../TableEntries/FloatEntry.h"
#include "../TableEntries/TypeNullEntry.h"
#include "../Utils/Utils.hpp"

#include <iostream>
#include <cmath> // real number powers

void Table::addEntry(const MyString& entryStr, size_t colIndex, size_t rowIndex, size_t lineCount)
{
    while (colIndex >= cols.size())
    {
        SharedPtr<TableCol> newCol(new TableCol(lineCount));
        cols.push_back(newCol);
    }
    SharedPtr<TableEntry> newEntry(TableEntryFactory::createEntry(entryStr));
    cols[colIndex]->setCell(rowIndex, newEntry);
}

void Table::executeAll()
{
    //reset errors
    visited.clear();
    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            SharedPtr<TableEntry> currEntry = cols[colIndex]->getCells()[rowIndex];
            if (currEntry->getType() == EntryType::ERROR)
            {
                const ErrorEntry* error = dynamic_cast<ErrorEntry*>(currEntry.get());
                SharedPtr<TableEntry> newEntry(TableEntryFactory::createEntry(error->getInputValue()));
                cols[colIndex]->setCell(rowIndex, newEntry);
            }
            else if (currEntry->getType() == EntryType::COMMAND)
            {
                CommandEntry* command = dynamic_cast<CommandEntry*>(currEntry.get());
                command->reset();
            }

        }
    }

    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            if (cols[colIndex]->getCells()[rowIndex]->getType() == EntryType::COMMAND)
            {
                execute(colIndex, rowIndex);
            }
        }
    }

    for (auto col : cols)
    {
        col->updateWidth();
    }

    printErrors();
}

void Table::execute(size_t colIndex, size_t rowIndex)
{
    SharedPtr<TableEntry> currEntry = cols[colIndex]->getCells()[rowIndex];
    CommandEntry* command = dynamic_cast<CommandEntry*>(currEntry.get());

    if (command->hasExecuted())
    {
        return;
    }

    if (Utils::contains(visited, currEntry))
    {
        makeCellError(colIndex, rowIndex, "Circular reference");
        return;
    }
    visited.push_back(currEntry);

    double result = 0;
    double lvalue = 0;
    double rvalue = 0;

    if (command->getIsLeftCell())
    {
        if (!parseCommandArg(colIndex, rowIndex, command->getLCIndex(), command->getLRIndex(), lvalue))
        {
            visited.pop_back();
            return;
        }
    }
    else
    {
        lvalue = command->getLNumberValue();
    }

    if (command->getIsRightCell())
    {
        if (!parseCommandArg(colIndex, rowIndex, command->getRCIndex(), command->getRRIndex(), rvalue))
        {
            visited.pop_back();
            return;
        }
    }
    else
    {
        rvalue = command->getRNumberValue();
    }

    switch (command->getOperation())
    {
    case Operation::ADD:
        result = lvalue + rvalue;
        break;
    case Operation::SUBTRACT:
        result = lvalue - rvalue;
        break;
    case Operation::MULTIPLY:
        result = lvalue * rvalue;
        break;
    case Operation::DIVIDE:
        if (rvalue == 0)
        {
            makeCellError(colIndex, rowIndex, "Division by zero");
            visited.pop_back();
            return;
        }
        result = lvalue / rvalue;
        break;
    case Operation::POWER:
        result = pow(lvalue, rvalue);
        break;
    default:
        break;
    }

    command->execute(result);
    visited.pop_back();

}

void Table::makeCellError(size_t colIndex, size_t rowIndex, const MyString& errorMsg)
{
    SharedPtr<TableEntry> currEntry = cols[colIndex]->getCells()[rowIndex];
    if (currEntry->getType() == EntryType::ERROR)
    {
        return;
    }
    SharedPtr<TableEntry> errorEntry(new ErrorEntry(currEntry->getInputValue(), errorMsg));
    cols[colIndex]->setCell(rowIndex, errorEntry);
}

void Table::printErrors() const
{
    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            if (cols[colIndex]->getCells()[rowIndex]->getType() == EntryType::ERROR)
            {
                const ErrorEntry* error = dynamic_cast<ErrorEntry*>(cols[colIndex]->getCells()[rowIndex].get());
                std::cout << 'R' << rowIndex << 'C' << colIndex << ": " << error->getErrorMsg() << std::endl;
            }
        }
    }
}

void Table::readInput(std::ifstream& in)
{
    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file");
    }
    size_t lineCount = Utils::GetLineCount(in);
    MyString line;
    size_t rowIndex = 0;
    size_t startIndex = 0;
    size_t endIndex = 0;
    while (line.getline(in))
    {
        startIndex = 0;
        endIndex = 0;
        size_t colIndex = 0;
        bool isString = false;
        while (endIndex < line.size())
        {
            if (line[endIndex] == '"' && (endIndex == 0 || line[endIndex - 1] != '\\'))
            {
                isString = !isString;
            }
            else if (line[endIndex] == ',' && !isString)
            {
                MyString cell = line.substr(startIndex, endIndex - startIndex);
                addEntry(cell, colIndex, rowIndex, lineCount);
                startIndex = endIndex + 1;
                colIndex++;
            }
            endIndex++;
        }
        MyString cell = line.substr(startIndex, endIndex - startIndex);
        addEntry(cell, colIndex, rowIndex, lineCount);
        rowIndex++;
    }
}

bool Table::parseCommandArg(size_t cColIndex, size_t cRowIndex, size_t colIndex, size_t rowIndex, double& result)
{
    if (colIndex == cColIndex && rowIndex == cRowIndex)
    {
        makeCellError(cColIndex, cRowIndex, "Self reference");
        return false;
    }
    if (colIndex >= cols.size() || rowIndex >= cols[colIndex]->getCells().size())
    {
        makeCellError(cColIndex, cRowIndex, "Invalid reference");
        return false;
    }
    if (cols[colIndex]->getCells()[rowIndex]->getType() == EntryType::COMMAND)
    {
        execute(colIndex, rowIndex);
    }
    if (cols[colIndex]->getCells()[rowIndex]->getType() == EntryType::ERROR)
    {
        const ErrorEntry* err = dynamic_cast<ErrorEntry*>(cols[colIndex]->getCells()[rowIndex].get());
        MyString errorMsg = err->getErrorMsg() + " in R" + MyString::fromInt(rowIndex) + "C" + MyString::fromInt(colIndex);
        makeCellError(cColIndex, cRowIndex, errorMsg);
        return false;
    }
    result = cols[colIndex]->getCells()[rowIndex]->getNumberValue();
    return true;
}

Table::Table(const MyString& filename)
{
    this->filename = filename;
    std::ifstream file(filename.c_str());

    readInput(file);

    file.close();

    executeAll();
}

Table::Table(std::ifstream& in)
{
    this->filename = in.getloc().name().c_str();
    readInput(in);
    executeAll();
}

void Table::read(const MyString& filename)
{
    std::ifstream file(filename.c_str());

    read(file);

    file.close();
}

void Table::read(std::ifstream& in)
{
    this->filename = in.getloc().name().c_str();

    readInput(in);
    executeAll();
}

void Table::save()
{
    if (filename.empty())
    {
        throw std::runtime_error("No filename specified");
    }
    save(filename);
}

void Table::save(const MyString& filename) const
{
    std::ofstream file(filename.c_str());
    save(file);
    file.close();
}

void Table::save(std::ofstream& out) const
{
    if (!out.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    if (cols.empty())
    {
        return;
    }
    SharedPtr<TableEntry> entry;
    for (size_t rowIndex = 0; rowIndex < cols[0]->getCells().size(); rowIndex++)
    {
        for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
        {
            entry = cols[colIndex]->getCells()[rowIndex];
            if (entry->getType() == EntryType::STRING)
            {
                out << '"';
                const MyString& str = entry->getInputValue();
                for (size_t i = 0; i < str.size(); i++)
                {
                    if (str[i] == '"')
                    {
                        out << '\\';
                    }
                    out << str[i];
                }
                out << '"';
            }
            else
            {
                out << entry->getInputValue();
            }
            if (colIndex != cols.size() - 1)
            {
                out << ',';
            }
        }
        if (rowIndex + 1 != cols[0]->getCells().size())
        {
            out << std::endl;
        }
    }
}

void Table::printInput() const
{
    if (cols.empty())
    {
        std::cout << "Empty table!" << std::endl;
        return;
    }
    std::cout << '+';
    for (size_t j = 0; j < cols.size(); j++)
    {
        std::cout << MyString(cols[j]->getOutputWidth() + 2, '-');
        std::cout << '+';
    }
    std::cout << std::endl;
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        std::cout << "| ";
        for (auto col : cols)
        {
            const MyString& output = col->getCells()[i]->getInputValue();
            std::cout << output << MyString(col->getOutputWidth() - output.size() + 1, ' ');
            std::cout << "| ";
        }
        std::cout << std::endl;
        std::cout << '+';
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << MyString(cols[j]->getOutputWidth() + 2, '-');
            std::cout << '+';
        }
        std::cout << std::endl;
    }
}

void Table::printNumberValues() const
{
    if (cols.empty())
    {
        std::cout << "Empty table!" << std::endl;
        return;
    }
    std::cout << '+';
    for (size_t j = 0; j < cols.size(); j++)
    {
        std::cout << MyString(cols[j]->getNumberWidth() + 2, '-'); // +2 for the spaces around the number
        std::cout << '+';
    }
    std::cout << std::endl;
    SharedPtr<TableEntry> entry;
    double output;
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        std::cout << "| ";
        for (auto col : cols)
        {
            entry = col->getCells()[i];
            output = entry->getNumberValue();
            EntryType type = entry->getType();
            if (type == EntryType::FLOAT)
            {
                const FloatEntry* floatEntry = dynamic_cast<FloatEntry*>(entry.get());
                Utils::setFloatPrecision(floatEntry->getDecimalPlaces());
            }
            else if (type == EntryType::COMMAND)
            {
                const CommandEntry* command = dynamic_cast<CommandEntry*>(entry.get());
                if (command->hasExecuted())
                {
                    Utils::setFloatPrecision(command->getDecimalPlaces());
                }
            }
            std::cout << output;
            std::cout << MyString(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            std::cout << "| ";

            Utils::resetFloatPrecision();

        }
        std::cout << std::endl;
        std::cout << '+';
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << MyString(cols[j]->getNumberWidth() + 2, '-');
            std::cout << '+';
        }
        std::cout << std::endl;
    }
}

void Table::printTypes() const
{
    EntryType type;
    if (cols.empty())
    {
        std::cout << "Empty table!" << std::endl;
        return;
    }
    std::cout << '+';
    for (size_t i = 0; i < cols.size(); i++)
    {
        std::cout << MyString(9, '-');
        std::cout << '+';
    }
    std::cout << std::endl;
    for (size_t rowIndex = 0; rowIndex < cols[0]->getCells().size(); rowIndex++)
    {
        std::cout << "|";
        for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
        {
            type = cols[colIndex]->getCells()[rowIndex]->getType();
            switch (type)
            {
            case EntryType::STRING:
                std::cout << " String  |";
                break;
            case EntryType::FLOAT:
                std::cout << " Float   |";
                break;
            case EntryType::COMMAND:
                std::cout << " Command |";
                break;
            case EntryType::INTEGER:
                std::cout << " Integer |";
                break;
            case EntryType::ERROR:
                std::cout << " Error   |";
                break;
            default:
                std::cout << " Null    |";
            }
        }
        std::cout << std::endl;
        std::cout << '+';
        for (size_t i = 0; i < cols.size(); i++)
        {
            std::cout << MyString(9, '-');
            std::cout << '+';
        }
        std::cout << std::endl;
    }
}

void Table::setCell(size_t row, size_t col, const MyString& value)
{
    if (row >= cols[0]->getCells().size() || col >= cols.size())
    {
        throw std::out_of_range("Invalid row or column");
    }
    SharedPtr<TableEntry> cell(TableEntryFactory::createEntry(value));
    cols[col]->setCell(row, cell);

    executeAll();
}

void Table::addCol()
{
    if (cols.empty())
    {
        cols.push_back(SharedPtr<TableCol>(new TableCol(1)));
        cols[0]->setCell(0, SharedPtr<TableEntry>(new TypeNullEntry()));
        return;
    }
    size_t size = cols[0]->getCells().size();
    cols.push_back(SharedPtr<TableCol>(new TableCol(size)));
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        cols.back()->addCell(SharedPtr<TableEntry>(new TypeNullEntry()));
    }

}

void Table::addRow()
{
    if (cols.empty())
    {
        cols.push_back(SharedPtr<TableCol>(new TableCol(1)));
    }
    for (auto col : cols)
    {
        col->addCell(SharedPtr<TableEntry>(new TypeNullEntry()));
    }
}

const MyString& Table::getFilename() const
{
    return filename;
}
