#include "Table.h"
#include "../TableEntryFactory/TableEntryFactory.h"
#include "../TableEntries/ErrorEntry.h"
#include "../TableEntries/FloatEntry.h"
#include "../TableEntries/TypeNullEntry.h"
#include "../Utils.hpp"

#include <iostream>
#include <cmath> // real number powers

void Table::copyFrom(const Table& other)
{
    for (const TableCol* col : other.cols)
    {
        cols.push_back(new TableCol(*col));
    }
    visited = MyVector<const TableEntry*>();
    filename = other.filename;
}

void Table::moveFrom(Table&& other)
{
    cols = std::move(other.cols);
    visited = std::move(other.visited);
    filename = std::move(other.filename);
}

void Table::free()
{
    for (const TableCol* col : cols)
    {
        delete col;
    }
}

void Table::addEntry(const MyString& entry, size_t colIndex, size_t rowIndex, size_t lineCount)
{
    while (colIndex >= cols.size())
    {
        cols.push_back(new TableCol(lineCount));
    }
    cols[colIndex]->setCell(rowIndex, TableEntryFactory::createEntry(entry));
}

void Table::executeAll()
{
    //reset errors
    visited.clear();
    TableEntry* currEntry;
    TableEntry* newEntry;
    const ErrorEntry* error;
    CommandEntry* command;
    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            currEntry = cols[colIndex]->getCells()[rowIndex];
            if (currEntry->getType() == EntryType::ERROR)
            {
                error = (ErrorEntry*)currEntry;
                newEntry = TableEntryFactory::createEntry(error->getInputValue());
                cols[colIndex]->setCell(rowIndex, newEntry);
            }
            else if (currEntry->getType() == EntryType::COMMAND)
            {
                command = (CommandEntry*)currEntry;
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

    for (TableCol* col : cols)
    {
        col->updateWidth();
    }

    printErrors();
}

void Table::execute(size_t colIndex, size_t rowIndex)
{
    TableEntry* currEntry = cols[colIndex]->getCells()[rowIndex];
    CommandEntry* command = (CommandEntry*)currEntry;

    if (command->hasExecuted())
    {
        return;
    }

    if (Utils::contains(visited, (const TableEntry*)currEntry))
    {
        makeCellError(colIndex, rowIndex, "Circular reference");
        return;
    }

    visited.push_back(cols[colIndex]->getCells()[rowIndex]);

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
    TableEntry* currEntry = cols[colIndex]->getCells()[rowIndex];
    if (currEntry->getType() == EntryType::ERROR)
    {
        return;
    }
    cols[colIndex]->setCell(rowIndex, new ErrorEntry(currEntry->getInputValue(), errorMsg));
}

void Table::printErrors() const
{
    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            if (cols[colIndex]->getCells()[rowIndex]->getType() == EntryType::ERROR)
            {
                ErrorEntry* error = (ErrorEntry*)(cols[colIndex]->getCells()[rowIndex]);
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
        ErrorEntry* err = (ErrorEntry*)(cols[colIndex]->getCells()[rowIndex]);
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

Table::Table(const Table& other)
{
    copyFrom(other);
}

Table::Table(Table&& other) noexcept
{
    moveFrom(std::move(other));
}

Table& Table::operator=(const Table& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

Table& Table::operator=(Table&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

Table::~Table()
{
    free();
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

    free();

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
    TableEntry* entry;
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
        for (const TableCol* col : cols)
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
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        std::cout << "| ";
        for (const TableCol* col : cols)
        {
            TableEntry* entry = col->getCells()[i];
            double output = entry->getNumberValue();
            EntryType type = entry->getType();
            if (type == EntryType::FLOAT)
            {
                Utils::setFloatPrecision(((FloatEntry*)entry)->getDecimalPlaces());
            }
            else if (type == EntryType::COMMAND)
            {
                const CommandEntry* command = (CommandEntry*)entry;
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
    TableEntry* cell = TableEntryFactory::createEntry(value);
    cols[col]->setCell(row, cell);

    executeAll();
}

void Table::addCol()
{
    if (cols.empty())
    {
        cols.push_back(new TableCol(1));
        cols[0]->setCell(0, new TypeNullEntry());
        return;
    }
    cols.push_back(new TableCol(cols[0]->getCells().size()));
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        cols.back()->addCell(new TypeNullEntry());
    }

}

void Table::addRow()
{
    if (cols.empty())
    {
        cols.push_back(new TableCol(1));
    }
    for (TableCol* col : cols)
    {
        col->addCell(new TypeNullEntry());
    }
}

const MyString& Table::getFilename() const
{
    return filename;
}
