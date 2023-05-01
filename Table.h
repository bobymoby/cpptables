#pragma once

#include <vector>
#include <fstream>
#include "TableCol.h"

class Table
{
private:
    std::vector<TableCol*> cols;

    void copyFrom(const Table& other);
    void free();

    void readUnsafe(std::ifstream& in);
public:
    Table();
    Table(const std::string& filename);
    Table(std::ifstream& in);
    Table(const Table& other);
    Table& operator=(const Table& other);
    ~Table();

    void read(const std::string& filename);
    void read(std::ifstream& in);

    void print() const;
    void printNumberValues() const;
};