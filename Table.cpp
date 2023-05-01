#include "Table.h"
#include "Utils.h"
#include <sstream>
#include <iostream>



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

void Table::readUnsafe(std::ifstream& in)
{
    size_t lineCount = Utils::GetLineCount(in);
    std::string line;
    size_t rowIndex = 0;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string cellStr;
        size_t colIndex = 0;
        while (std::getline(iss, cellStr, ','))
        {
            Utils::strip(cellStr, ' ');
            if (rowIndex == 0)
            {
                cols.push_back(new TableCol(lineCount));
            }
            cols[colIndex]->setCell(rowIndex, TableEntryFactory::createEntry(cellStr));
            colIndex++;
        }
        rowIndex++;
        if (line.back() == ',')
        {
            cols.push_back(new TableCol(lineCount));
        }
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

    file.close();
}

Table::Table(std::ifstream& in)
{
    if (!in.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    readUnsafe(in);
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
}

void Table::print() const
{
    std::cout << "|";
    for (size_t j = 0; j < cols.size(); j++)
    {
        std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
        if (j != cols.size() - 1)
        {
            std::cout << "+";
        }
        else
        {
            std::cout << "|";
        }
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
        std::cout << "|";
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
            if (j != cols.size() - 1)
            {
                std::cout << "+";
            }
            else
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
    }
}

void Table::printNumberValues() const
{
    std::cout << "|";
    for (size_t j = 0; j < cols.size(); j++)
    {
        std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
        if (j != cols.size() - 1)
        {
            std::cout << "+";
        }
        else
        {
            std::cout << "|";
        }
    }
    std::cout << std::endl;
    for (size_t i = 0; i < cols[0]->getCells().size(); i++)
    {
        std::cout << "| ";
        for (const TableCol* col : cols)
        {
            double output = col->getCells()[i]->getNumberValue();
            std::cout << output;
            // std::cout << output << std::string(col->getOutputWidth() - Utils::NumberLength(output) + 1, ' ');
            std::cout << "| ";
        }
        std::cout << std::endl;
        std::cout << "|";
        for (size_t j = 0; j < cols.size(); j++)
        {
            std::cout << std::string(cols[j]->getOutputWidth() + 2, '-');
            if (j != cols.size() - 1)
            {
                std::cout << "+";
            }
            else
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
    }
}
