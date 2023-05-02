#include "Table.h"
#include "TableEntryFactory/TableEntryFactory.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

void Table::copyFrom(const Table& other)
{
    for (TableCol* col : other.cols)
    {
        cols.push_back(new TableCol(*col));
    }
}

void Table::free()
{
    for (TableCol* col : cols)
    {
        delete col;
    }
}

void Table::addEntry(std::string& entry, size_t colIndex, size_t rowIndex, size_t lineCount)
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
    CommandEntry* command = (CommandEntry*)(cols[colIndex]->getCells()[rowIndex]);

    if (command->hasExecuted())
    {
        return;
    }

    double result = 0;
    size_t lcolIndex = command->getLCIndex();
    size_t lrowIndex = command->getLRIndex();
    size_t rcolIndex = command->getRCIndex();
    size_t rrowIndex = command->getRRIndex();

    if (lcolIndex >= cols.size())
    {
        std::cout << 'R' << rowIndex + 1 << 'C' << colIndex + 1 << " :";
        std::cout << "Invalid left column index: " << lcolIndex << std::endl;
        return;
    }
    if (lrowIndex >= cols[lcolIndex]->getCells().size())
    {
        std::cout << 'R' << rowIndex + 1 << 'C' << colIndex + 1 << " :";
        std::cout << "Invalid left row index: " << lrowIndex << std::endl;
        return;
    }
    if (rcolIndex >= cols.size())
    {
        std::cout << 'R' << rowIndex + 1 << 'C' << colIndex + 1 << " :";
        std::cout << "Invalid right column index: " << rcolIndex << std::endl;
        return;
    }
    if (rrowIndex >= cols[rcolIndex]->getCells().size())
    {
        std::cout << 'R' << rowIndex + 1 << 'C' << colIndex + 1 << " :";
        std::cout << "Invalid right row index: " << rrowIndex << std::endl;
        return;
    }

    double lvalue = cols[lcolIndex]->getCells()[lrowIndex]->getNumberValue();
    double rvalue = cols[rcolIndex]->getCells()[rrowIndex]->getNumberValue();

    switch (command->getOperation())
    {
    case Operation::Add:
        result = lvalue + rvalue;
        break;
    case Operation::Subtract:
        result = lvalue - rvalue;
        break;
    case Operation::Multiply:
        result = lvalue * rvalue;
        break;
    case Operation::Divide:
        result = lvalue / rvalue;
        break;
    default:
        break;
    }

    command->execute(result);
}

void Table::readUnsafe(std::ifstream& in)
{
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
            if (line[endIndex] == '"')
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

Table::Table(): cols()
{
}

Table::Table(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + filename);
    }

    readUnsafe(file);

    executeAll();

    file.close();
}

Table::Table(std::ifstream& in)
{
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    readUnsafe(in);
    executeAll();
}

Table::Table(const Table& other)
{
    copyFrom(other);
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

Table::~Table()
{
    free();
}

void Table::read(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + filename);
    }

    free();

    readUnsafe(file);
    executeAll();

    file.close();
}

void Table::read(std::ifstream& in)
{
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    free();

    readUnsafe(in);
    executeAll();
}

void Table::print() const
{
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
    std::cout << std::setprecision(2) << std::fixed;
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
            std::cout << output << std::string(col->getNumberWidth() - entry->getNumberWidth() + 1, ' ');
            std::cout << "| ";
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
