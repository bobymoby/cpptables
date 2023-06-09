#pragma once

#include "../Table/Table.h"

class MainScreen
{
private:
    Table _table;
    bool usingDynamicTable;

    bool isTableOpen() const;

public:
    MainScreen();

    void run();
};