#pragma once

#include <vector>

#include "TableEntry.h"

class TableCol
{
private:
    std::vector<TableEntry*> cells;
    unsigned int outputWidth;
    unsigned int numberWidth;

    void updateWidth();

    void copyFrom(const TableCol& other);
    void free();
public:
    TableCol(const std::vector<TableEntry*>& cells);
    TableCol(size_t size);
    ~TableCol();
    TableCol(const TableCol& other);
    TableCol& operator=(const TableCol& other);

    const std::vector<TableEntry*>& getCells() const;
    void setCell(size_t index, TableEntry* cell);
    void addCell(TableEntry* cell);
    unsigned int getOutputWidth() const;
    unsigned int getNumberWidth() const;
};