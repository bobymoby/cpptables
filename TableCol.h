#pragma once

#include <vector>

#include "TableEntries/TableEntry.h"
#include "TableEntries/CommandEntry.h"

class TableCol
{
private:
    std::vector<TableEntry*> cells;
    unsigned int outputWidth;
    unsigned int numberWidth;


    void free();
    void copyFrom(const TableCol& other);
    void moveFrom(TableCol&& other);
public:
    TableCol(const std::vector<TableEntry*>& cells);
    TableCol(size_t size);

    TableCol();
    TableCol(const TableCol& other);
    TableCol(TableCol&& other) noexcept;
    TableCol& operator=(const TableCol& other);
    TableCol& operator=(TableCol&& other) noexcept;
    ~TableCol();

    const std::vector<TableEntry*>& getCells() const;
    void setCell(size_t index, TableEntry* cell);
    void addCell(TableEntry* cell);
    unsigned int getOutputWidth() const;
    unsigned int getNumberWidth() const;
    void updateWidth();
};