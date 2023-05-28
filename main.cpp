#include "Table.h"
#include <iostream>

int main()
{
    Table table("table.txt");
    table.print();
    table.printNumberValues();

    table.save("table2.txt");

    // Table table2("stressTable.txt");
    // table2.print();
    // table2.printNumberValues();
    return 0;
}