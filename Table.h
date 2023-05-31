#pragma once

#include <vector>
#include <fstream>
#include "TableCol.h"

class Table
{
private:
    std::vector<TableCol*> cols;
    std::string filename;

    std::vector<const TableEntry*> visited; //used for detecting circular dependencies

    void free();
    void copyFrom(const Table& other);
    void moveFrom(Table&& other);

    void addEntry(const std::string& entry, size_t colIndex, size_t rowIndex, size_t lineCount);

    void executeAll();
    void execute(size_t colIndex, size_t rowIndex);

    void makeCellError(size_t colIndex, size_t rowIndex, const std::string& errorMsg);
    void printErrors() const;

    void readInput(std::ifstream& in);

public:
    Table();
    Table(const std::string& filename);
    Table(std::ifstream& in);
    Table(const Table& other);
    Table(Table&& other) noexcept;
    Table& operator=(const Table& other);
    Table& operator=(Table&& other) noexcept;
    ~Table();

    void read(const std::string& filename);
    void read(std::ifstream& in);

    void save();
    void save(const std::string& filename) const;
    void save(std::ofstream& out) const;

    void print() const;
    void printNumberValues() const;
};