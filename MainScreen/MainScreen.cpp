#include "MainScreen.h"
#include "Utils.hpp"



void MainScreen::run()
{
    MyString command;
    while (true)
    {
        std::cout << "> ";
        // std::cin >> command;

        if (Utils::strcmp(command.c_str(), "exit") == 0)
        {
            break;
        }
        if (Utils::strcmp(command.c_str(), "load") == 0)
        {
            MyString fileName;
            std::cout << "Enter file name: ";
            // std::cin >> fileName;
            _table = Table(fileName);
        }
        if (Utils::strcmp(command.c_str(), "save") == 0)
        {
            MyString fileName;
            std::cout << "Enter file name: ";
            // std::cin >> fileName;
            _table.save(fileName);
        }
        if (Utils::strcmp(command.c_str(), "print") == 0)
        {
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Input values:" << std::endl;
            _table.print();
            std::cout << "Output values:" << std::endl;
            _table.printNumberValues();
        }
        if (Utils::strcmp(command.c_str(), "setcell") == 0)
        {
            size_t row, col;
            std::cout << "Enter row(starts from 0):";
            // std::cin >> row;
            std::cout << "Enter col(starts from 0): ";
            // std::cin >> col;
            MyString value;
            std::cout << "Enter value: ";
            // std::cin >> value;
            _table.setCell(row, col, value);
        }
    }
}