#pragma once

#include <vector>
#include <fstream>
#include "TableCol.h"

class Table
{
private:
    MyVector<TableCol*> cols;
    MyString filename;

    MyVector<const TableEntry*> visited; //used for detecting circular dependencies

    void free();
    void copyFrom(const Table& other);
    void moveFrom(Table&& other);

    void addEntry(const MyString& entry, size_t colIndex, size_t rowIndex, size_t lineCount);

    void executeAll();
    void execute(size_t colIndex, size_t rowIndex);

    void makeCellError(size_t colIndex, size_t rowIndex, const MyString& errorMsg);
    void printErrors() const;

    void readInput(std::ifstream& in);

    bool parseCommandArg(size_t cColIndex, size_t cRowIndex, size_t colIndex, size_t rowIndex, double& result); //c = command - used for error messages

public:
    Table();
    Table(const MyString& filename);
    Table(std::ifstream& in);
    Table(const Table& other);
    Table(Table&& other) noexcept;
    Table& operator=(const Table& other);
    Table& operator=(Table&& other) noexcept;
    ~Table();

    void read(const MyString& filename);
    void read(std::ifstream& in);

    void save();
    void save(const MyString& filename) const;
    void save(std::ofstream& out) const;

    void print() const;
    void printNumberValues() const;
    void printTypes() const;

    void setCell(size_t row, size_t col, const MyString& value);
    void addRow();
    void addCol();


    const MyString& getFilename() const;
};