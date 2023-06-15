#pragma once

#include "../TableEntries/TableEntry.h"
#include "../TableEntries/CommandEntry.h"
#include "../MyVector/MyVector.hpp"
#include "../SharedPtr/SharedPtr.hpp"

class TableCol
{
private:
    MyVector<SharedPtr<TableEntry>> cells;
    unsigned int outputWidth;
    unsigned int numberWidth;

public:
    TableCol() = default;

    TableCol(const MyVector<SharedPtr<TableEntry>>& cells);
    TableCol(size_t size);

    const MyVector<SharedPtr<TableEntry>>& getCells() const;
    void setCell(size_t index, const SharedPtr<TableEntry>& cell);
    void addCell(const SharedPtr<TableEntry>& cell);
    unsigned int getOutputWidth() const;
    unsigned int getNumberWidth() const;
    void updateWidth();
};