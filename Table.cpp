#include "Table.h"
#include "TableEntryFactory/TableEntryFactory.h"
#include "TableEntries/ErrorEntry.h"
#include "TableEntries/FloatEntry.h"
#include "Utils.hpp"

#include <iostream>
#include <cmath>
#include <iomanip>

void Table::copyFrom(const Table& other)
{
    for (TableCol* col : other.cols)
    {
        cols.push_back(new TableCol(*col));
    }
    visited.clear();
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
    for (TableCol* col : cols)
    {
        delete col;
    }
}

void Table::addEntry(const MyString& entry, size_t colIndex, size_t rowIndex, size_t lineCount)
{
    // Utils::strip(entry, ' ');
    while (colIndex >= cols.size())
    {
        cols.push_back(new TableCol(lineCount));
    }
    cols[colIndex]->setCell(rowIndex, TableEntryFactory::createEntry(entry));
}

void Table::executeAll()
{
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
}

void Table::execute(size_t colIndex, size_t rowIndex)
{
    TableEntry* currEntry = cols[colIndex]->getCells()[rowIndex];
    CommandEntry* command = dynamic_cast<CommandEntry*>(currEntry);

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
    size_t lcolIndex;
    size_t lrowIndex;
    size_t rcolIndex;
    size_t rrowIndex;

    if (command->getIsLeftCell())
    {
        lcolIndex = command->getLCIndex();
        lrowIndex = command->getLRIndex();
        if (lcolIndex == colIndex && lrowIndex == rowIndex)
        {
            makeCellError(colIndex, rowIndex, "Self reference");
            visited.pop_back();
            return;
        }
        if (lcolIndex >= cols.size() || lrowIndex >= cols[lcolIndex]->getCells().size())
        {
            makeCellError(colIndex, rowIndex, "Invalid reference");
            visited.pop_back();
            return;
        }
        if (cols[lcolIndex]->getCells()[lrowIndex]->getType() == EntryType::COMMAND)
        {
            execute(lcolIndex, lrowIndex);
        }
        if (cols[lcolIndex]->getCells()[lrowIndex]->getType() == EntryType::ERROR)
        {
            ErrorEntry* err = dynamic_cast<ErrorEntry*>(cols[lcolIndex]->getCells()[lrowIndex]);
            MyString errorMsg = err->getErrorMsg() + " in R" + std::to_string(lrowIndex) + "C" + std::to_string(lcolIndex);
            makeCellError(colIndex, rowIndex, errorMsg);
            visited.pop_back();
            return;
        }
        lvalue = cols[lcolIndex]->getCells()[lrowIndex]->getNumberValue();
    }
    else
    {
        lvalue = command->getLNumberValue();
    }

    if (command->getIsRightCell())
    {
        rcolIndex = command->getRCIndex();
        rrowIndex = command->getRRIndex();
        if (rcolIndex == colIndex && rrowIndex == rowIndex)
        {
            makeCellError(colIndex, rowIndex, "Self reference");
            visited.pop_back();
            return;
        }
        if (rcolIndex >= cols.size() || rrowIndex >= cols[rcolIndex]->getCells().size())
        {
            makeCellError(colIndex, rowIndex, "Invalid reference");
            visited.pop_back();
            return;
        }
        if (cols[rcolIndex]->getCells()[rrowIndex]->getType() == EntryType::COMMAND)
        {
            execute(rcolIndex, rrowIndex);
        }
        if (cols[rcolIndex]->getCells()[rrowIndex]->getType() == EntryType::ERROR)
        {
            ErrorEntry* err = dynamic_cast<ErrorEntry*>(cols[rcolIndex]->getCells()[rrowIndex]);
            MyString errorMsg = err->getErrorMsg() + " in R" + std::to_string(rrowIndex) + "C" + std::to_string(rcolIndex);
            makeCellError(colIndex, rowIndex, errorMsg);
            visited.pop_back();
            return;
        }
        rvalue = cols[rcolIndex]->getCells()[rrowIndex]->getNumberValue();
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
        throw std::runtime_error("File not open!");
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

Table::Table() : cols()
{
}

Table::Table(const MyString& filename)
{
    this->filename = filename;
    std::ifstream file(filename.c_str());

    readInput(file);

    file.close();

    executeAll();

    printErrors();
}

Table::Table(std::ifstream& in)
{
    this->filename = in.getloc().name();
    readInput(in);
    executeAll();

    printErrors();
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
    this->filename = in.getloc().name();

    free();

    readInput(in);
    executeAll();
}

void Table::save()
{
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

    for (size_t rowIndex = 0; rowIndex < cols[0]->getCells().size(); rowIndex++)
    {
        for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
        {
            out << cols[colIndex]->getCells()[rowIndex]->getInputValue();
            if (colIndex != cols.size() - 1)
            {
                out << ',';
            }
        }
        out << std::endl;
    }
}

void Table::print() const
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
        std::cout << MyString(cols[j]->getNumberWidth() + 2, '-');
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
            if (entry->getType() == EntryType::FLOAT)
            {
                std::cout << std::setprecision(((FloatEntry*)entry)->getDecimalPlaces());
                std::cout << std::fixed;
            }
            std::cout << output;
            std::cout << MyString(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            // std::cout << output << MyString(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            std::cout << "| ";

            std::cout << std::setprecision(6);
            std::cout << std::defaultfloat;

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
