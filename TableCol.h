#pragma once

#include <vector>

#include "TableEntries/TableEntry.h"
#include "TableEntries/CommandEntry.h"
#include "MyVector/MyVector.hpp"

class TableCol
{
private:
    MyVector<TableEntry*> cells;
    unsigned int outputWidth;
    unsigned int numberWidth;


    void free();
    void copyFrom(const TableCol& other);
    void moveFrom(TableCol&& other);
public:
    TableCol(const MyVector<TableEntry*>& cells);
    TableCol(size_t size);

    TableCol();
    TableCol(const TableCol& other);
    TableCol(TableCol&& other) noexcept;
    TableCol& operator=(const TableCol& other);
    TableCol& operator=(TableCol&& other) noexcept;
    ~TableCol();

    const MyVector<TableEntry*>& getCells() const;
    void setCell(size_t index, TableEntry* cell);
    void addCell(TableEntry* cell);
    unsigned int getOutputWidth() const;
    unsigned int getNumberWidth() const;
    void updateWidth();
};