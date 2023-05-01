#include "TableCol.h"
#include <iostream>



void TableCol::updateWidth()
{
    outputWidth = 0;
    numberWidth = 0;
    for (const TableEntry* cell : cells)
    {
        if (cell->getOutputWidth() > outputWidth)
        {
            outputWidth = cell->getOutputWidth();
        }
        if (cell->getNumberWidth() > numberWidth)
        {
            numberWidth = cell->getNumberWidth();
        }
    }
}

void TableCol::copyFrom(const TableCol& other)
{
    outputWidth = other.outputWidth;
    numberWidth = other.numberWidth;
    for (const TableEntry* cell : other.cells)
    {
        cells.push_back(new TableEntry(*cell));
    }
}

void TableCol::free()
{
    for (const TableEntry* cell : cells)
    {
        delete cell;
    }
}

TableCol::TableCol(const std::vector<TableEntry*>& cells): cells(cells)
{
    updateWidth();
}

TableCol::TableCol(size_t size)
{
    cells.resize(size, TableEntryFactory::createEntry(""));
}

TableCol::~TableCol()
{
    free();
}

TableCol::TableCol(const TableCol& other)
{
    copyFrom(other);
}

TableCol& TableCol::operator=(const TableCol& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

const std::vector<TableEntry*>& TableCol::getCells() const
{
    return cells;
}

void TableCol::setCell(size_t index, TableEntry* cell)
{
    cells[index] = cell;
    updateWidth();
}

void TableCol::addCell(TableEntry* cell)
{
    cells.push_back(cell);
    if (cell->getOutputWidth() > outputWidth)
    {
        outputWidth = cell->getOutputWidth();
    }
    if (cell->getNumberWidth() > numberWidth)
    {
        numberWidth = cell->getNumberWidth();
    }
}

unsigned int TableCol::getOutputWidth() const
{
    return outputWidth;
}

unsigned int TableCol::getNumberWidth() const
{
    return numberWidth;
}
