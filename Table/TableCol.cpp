#include "TableCol.h"
#include "../TableEntryFactory/TableEntryFactory.h"
#include "../TableEntries/TypeNullEntry.h"

#include <iostream>


void TableCol::updateWidth()
{
    outputWidth = 0;
    numberWidth = 0;
    for (auto cell : cells)
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

TableCol::TableCol(const MyVector<SharedPtr<TableEntry>>& cells) : cells(cells)
{
    updateWidth();
}

TableCol::TableCol(size_t size)
{
    cells.fill(size, SharedPtr<TableEntry>(nullptr));
    for (size_t i = 0; i < size; i++)
    {
        cells[i] = SharedPtr<TableEntry>(new TypeNullEntry());
    }
}

const MyVector<SharedPtr<TableEntry>>& TableCol::getCells() const
{
    return cells;
}

void TableCol::setCell(size_t index, const SharedPtr<TableEntry>& cell)
{
    cells[index] = cell;
    updateWidth();
}

void TableCol::addCell(const SharedPtr<TableEntry>& cell)
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