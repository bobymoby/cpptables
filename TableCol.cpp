#include "TableCol.h"
#include <iostream>



void TableCol::updateWidth()
{
    outputWidth = 0;
    for (const TableEntry* cell : cells)
    {
        if (cell->getOutputWidth() > outputWidth)
        {
            outputWidth = cell->getOutputWidth();
        }
    }
}

void TableCol::copyFrom(const TableCol& other)
{
    outputWidth = other.outputWidth;
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
}

unsigned int TableCol::getOutputWidth() const
{
    return outputWidth;
}
