#include "MainScreen.h"
#include "../Utils.hpp"



void MainScreen::run()
{
    MyString command;
    while (true)
    {
        std::cout << "> ";
        std::cin >> command;

        const char* commandStr = command.c_str();

        if (Utils::strcmp(commandStr, "exit") == 0)
        {
            break;
        }
        else if (Utils::strcmp(commandStr, "load") == 0)
        {
            MyString fileName;
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            _table = Table(fileName);
        }
        else if (Utils::strcmp(commandStr, "save") == 0)
        {
            MyString fileName;
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            _table.save(fileName);
        }
        else if (Utils::strcmp(commandStr, "print") == 0)
        {
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Input values:" << std::endl;
            _table.print();
            std::cout << "Output values:" << std::endl;
            _table.printNumberValues();
            std::cout << "Types:" << std::endl;
            _table.printTypes();
        }
        else if (Utils::strcmp(commandStr, "setcell") == 0)
        {
            size_t row, col;
            std::cout << "Enter row(starts from 0):";
            std::cin >> row;
            std::cout << "Enter col(starts from 0): ";
            std::cin >> col;
            MyString value;
            std::cout << "Enter value: ";
            std::cin.ignore();
            std::cin >> value;
            _table.setCell(row, col, value);
        }
        else if (Utils::strcmp(commandStr, "addrow") == 0)
        {
            _table.addRow();
        }
        else if (Utils::strcmp(commandStr, "addcol") == 0)
        {
            _table.addCol();
        }
        else if (Utils::strcmp(commandStr, "help") == 0)
        {
            std::cout << "exit - exits the program" << std::endl;
            std::cout << "load - loads a table from a file" << std::endl;
            std::cout << "save - saves the table to a file" << std::endl;
            std::cout << "print - prints the table" << std::endl;
            std::cout << "setcell - sets a cell value" << std::endl;
            std::cout << "addrow - adds a row" << std::endl;
            std::cout << "addcol - adds a column" << std::endl;
            std::cout << "help - prints this message" << std::endl;
        }
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    }
}