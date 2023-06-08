#include "Table.h"
#include "TableEntryFactory/TableEntryFactory.h"
#include "TableEntries/ErrorEntry.h"
#include "TableEntries/FloatEntry.h"
#include "TableEntries/TypeNullEntry.h"
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
    //reset errors
    visited.clear();
    TableEntry* currEntry;
    for (size_t colIndex = 0; colIndex < cols.size(); colIndex++)
    {
        for (size_t rowIndex = 0; rowIndex < cols[colIndex]->getCells().size(); rowIndex++)
        {
            currEntry = cols[colIndex]->getCells()[rowIndex];
            if (currEntry->getType() == EntryType::ERROR)
            {
                ErrorEntry* error = dynamic_cast<ErrorEntry*>(currEntry);
                TableEntry* newEntry = TableEntryFactory::createEntry(error->getInputValue());
                cols[colIndex]->setCell(rowIndex, newEntry);
            }
            else if (currEntry->getType() == EntryType::COMMAND)
            {
                CommandEntry* command = dynamic_cast<CommandEntry*>(currEntry);
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

    if (command->getIsLeftCell())
    {
        if (parseCommandArg(colIndex, rowIndex, command->getLCIndex(), command->getLRIndex(), lvalue))
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
        if (parseCommandArg(colIndex, rowIndex, command->getRCIndex(), command->getRRIndex(), rvalue))
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
        ErrorEntry* err = dynamic_cast<ErrorEntry*>(cols[colIndex]->getCells()[rowIndex]);
        MyString errorMsg = err->getErrorMsg() + " in R" + std::to_string(rowIndex) + "C" + std::to_string(colIndex);
        makeCellError(cColIndex, cRowIndex, errorMsg);
        return false;
    }
    result = cols[colIndex]->getCells()[rowIndex]->getNumberValue();
    return true;
}

Table::Table() : cols(), filename(), visited()
{
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

void Table::printTypes() const
{
    EntryType type;
    for (size_t i = 0; i < cols.size(); i++)
    {
        for (size_t j = 0; j < cols[i]->getCells().size(); j++)
        {
            type = cols[i]->getCells()[j]->getType();
            switch (type)
            {
            case EntryType::STRING:
                std::cout << "S";
                break;
            case EntryType::FLOAT:
                std::cout << "F";
                break;
            case EntryType::COMMAND:
                std::cout << "C";
                break;
            case EntryType::INTEGER:
                std::cout << "I";
                break;
            case EntryType::ERROR:
                std::cout << "E";
                break;
            default:
                std::cout << "N";
            }
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
        return;
    }
    cols.push_back(new TableCol(cols[0]->getCells().size()));
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
