#include "TableCol.h"
#include "../TableEntryFactory/TableEntryFactory.h"
#include "../TableEntries/TypeNullEntry.h"

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
        cells.push_back(cell->clone());
    }
}

void TableCol::moveFrom(TableCol&& other)
{
    outputWidth = other.outputWidth;
    numberWidth = other.numberWidth;
    cells = std::move(other.cells);
}

void TableCol::free()
{
    for (const TableEntry* cell : cells)
    {
        delete cell;
    }
}

TableCol::TableCol(const MyVector<TableEntry*>& cells) : cells(cells)
{
    updateWidth();
}

TableCol::TableCol(size_t size)
{
    cells.fill(size, nullptr);
    for (size_t i = 0; i < size; i++)
    {
        cells[i] = new TypeNullEntry();
    }
}

TableCol::~TableCol()
{
    free();
}

TableCol::TableCol()
{
    outputWidth = 0;
    numberWidth = 0;
}

TableCol::TableCol(const TableCol& other)
{
    copyFrom(other);
}

TableCol::TableCol(TableCol&& other) noexcept
{
    moveFrom(std::move(other));
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

TableCol& TableCol::operator=(TableCol&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

const MyVector<TableEntry*>& TableCol::getCells() const
{
    return cells;
}

void TableCol::setCell(size_t index, TableEntry* cell)
{
    delete cells[index];
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
