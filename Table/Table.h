#pragma once

#include "../SharedPtr/SharedPtr.hpp"
#include "TableCol.h"

#include <fstream>


class Table
{
private:

    MyVector<SharedPtr<TableCol>> cols;
    MyString filename;

    MyVector<SharedPtr<TableEntry>> visited; //used for detecting circular dependencies

    void addEntry(const MyString& entryStr, size_t colIndex, size_t rowIndex, size_t lineCount);

    void executeAll();
    void execute(size_t colIndex, size_t rowIndex);

    void makeCellError(size_t colIndex, size_t rowIndex, const MyString& errorMsg);
    void printErrors() const;

    void readInput(std::ifstream& in);

    bool parseCommandArg(size_t cColIndex, size_t cRowIndex, size_t colIndex, size_t rowIndex, double& result); //c = command - used for error messages

public:

    Table() = default;
    Table(const MyString& filename);
    Table(std::ifstream& in);

    void read(const MyString& filename);
    void read(std::ifstream& in);

    void save();
    void save(const MyString& filename) const;
    void save(std::ofstream& out) const;

    void printInput() const;
    void printNumberValues() const;
    void printTypes() const;

    void setCell(size_t row, size_t col, const MyString& value);
    void addRow();
    void addCol();

    const MyString& getFilename() const;

};