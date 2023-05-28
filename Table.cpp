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
}

void Table::moveFrom(Table&& other)
{
    cols = std::move(other.cols);
}

void Table::free()
{
    for (TableCol* col : cols)
    {
        delete col;
    }
}

void Table::addEntry(const std::string& entry, size_t colIndex, size_t rowIndex, size_t lineCount)
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

    if (Utils::contains(visited, (const TableEntry*)currEntry))
    {
        makeCellError(colIndex, rowIndex, command->getInputValue() + " Circular reference");
        return;
    }

    visited.push_back(cols[colIndex]->getCells()[rowIndex]);

    if (command->hasExecuted())
    {
        return;
    }

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
            makeCellError(colIndex, rowIndex, command->getInputValue() + " Self reference");
            visited.pop_back();
            return;
        }
        if (lcolIndex >= cols.size() || lrowIndex >= cols[lcolIndex]->getCells().size())
        {
            makeCellError(colIndex, rowIndex, command->getInputValue() + " Invalid reference");
            visited.pop_back();
            return;
        }
        if (cols[lcolIndex]->getCells()[lrowIndex]->getType() == EntryType::COMMAND)
        {
            execute(lcolIndex, lrowIndex);
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
            makeCellError(colIndex, rowIndex, command->getInputValue() + " Self reference");
            visited.pop_back();
            return;
        }
        if (rcolIndex >= cols.size() || rrowIndex >= cols[rcolIndex]->getCells().size())
        {
            makeCellError(colIndex, rowIndex, command->getInputValue() + " Invalid reference");
            visited.pop_back();
            return;
        }
        if (cols[rcolIndex]->getCells()[rrowIndex]->getType() == EntryType::COMMAND)
        {
            execute(rcolIndex, rrowIndex);
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
            makeCellError(colIndex, rowIndex, command->getInputValue() + " Division by zero");
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

void Table::makeCellError(size_t colIndex, size_t rowIndex, const std::string& errorMsg)
{
    cols[colIndex]->setCell(rowIndex, new ErrorEntry(errorMsg));
}

void Table::readInput(std::ifstream& in)
{
    if (!in.is_open())
    {
        throw std::runtime_error("File not open!");
    }
    size_t lineCount = Utils::GetLineCount(in);
    std::string line;
    size_t rowIndex = 0;
    size_t startIndex = 0;
    size_t endIndex = 0;
    while (std::getline(in, line))
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
                std::string cell = line.substr(startIndex, endIndex - startIndex);
                addEntry(cell, colIndex, rowIndex, lineCount);
                startIndex = endIndex + 1;
                colIndex++;
            }
            endIndex++;
        }
        std::string cell = line.substr(startIndex, endIndex - startIndex);
        addEntry(cell, colIndex, rowIndex, lineCount);
        rowIndex++;
    }
}

Table::Table() : cols()
{
}

Table::Table(const std::string& filename)
{
    this->filename = filename;
    std::ifstream file(filename);

    readInput(file);

    file.close();

    executeAll();
}

Table::Table(std::ifstream& in)
{
    this->filename = in.getloc().name();
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

void Table::read(const std::string& filename)
{
    std::ifstream file(filename);

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

void Table::save(const std::string& filename) const
{
    std::ofstream file(filename);
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
        std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
        std::cout << '+';
    }
    std::cout << std::endl;
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        std::cout << "| ";
        for (const TableCol* col : cols)
        {
            const std::string& output = col->getCells()[i]->getInputValue();
            std::cout << output << std::string(col->getOutputWidth() - output.size() + 1, ' ');
            std::cout << "| ";
        }
        std::cout << std::endl;
        std::cout << '+';
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
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
        std::cout << std::string(cols[j]->getNumberWidth() + 2, '-');
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
            std::cout << std::string(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            // std::cout << output << std::string(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            std::cout << "| ";

            std::cout << std::setprecision(6);
            std::cout << std::defaultfloat;

        }
        std::cout << std::endl;
        std::cout << '+';
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << std::string(cols[j]->getNumberWidth() + 2, '-');
            std::cout << '+';
        }
        std::cout << std::endl;
    }
}
