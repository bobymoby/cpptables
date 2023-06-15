#include "MainScreen.h"
#include "../Utils/Utils.hpp"



bool MainScreen::isTableOpen() const
{
    return usingDynamicTable || _table.getFilename().size() > 0;
}

void MainScreen::resetTable()
{
    _table = Table();
    _table.addCol();
    usingDynamicTable = true;
}

MainScreen::MainScreen()
{
    resetTable();
}

void MainScreen::run()
{
    MyString command;
    while (true)
    {
        std::cout << "> ";
        std::cin >> command;

        const char* commandStr = command.c_str();

        if (Utils::strcmp(commandStr, "exit") == 0
            || Utils::strcmp(commandStr, "quit") == 0)
        {
            break;
        }
        else if (Utils::strcmp(commandStr, "load") == 0)
        {
            MyString fileName;
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            try
            {
                _table = Table(fileName);
                usingDynamicTable = false;
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                usingDynamicTable = true;
            }
        }
        else if (Utils::strcmp(commandStr, "save") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            MyString fileName;
            std::cout << "Enter file name: ";
            std::cin >> fileName;
            try
            {
                _table.save(fileName);
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }
        else if (Utils::strcmp(commandStr, "print") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Input values:" << std::endl;
            _table.printInput();
            std::cout << "Output values:" << std::endl;
            _table.printNumberValues();
            std::cout << "Types:" << std::endl;
            _table.printTypes();
        }
        else if (Utils::strcmp(commandStr, "printInput") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Input values:" << std::endl;
            _table.printInput();
        }
        else if (Utils::strcmp(commandStr, "printOutput") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Output values:" << std::endl;
            _table.printNumberValues();
        }
        else if (Utils::strcmp(commandStr, "printTypes") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            std::cout << "Table: " << _table.getFilename() << std::endl;
            std::cout << "Types:" << std::endl;
            _table.printTypes();
        }
        else if (Utils::strcmp(commandStr, "setcell") == 0)
        {
            if (!isTableOpen())
            {
                std::cout << "There is no open table" << std::endl;
                continue;
            }
            size_t row, col;
            std::cout << "Enter row: ";
            std::cin >> row;
            std::cout << "Enter col: ";
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
        else if (Utils::strcmp(commandStr, "reset") == 0)
        {
            resetTable();
        }
        else if (Utils::strcmp(commandStr, "help") == 0)
        {
            std::cout << "exit - exits the program" << std::endl;
            std::cout << "load - loads a table from a file" << std::endl;
            std::cout << "save - saves the table to a file" << std::endl;
            std::cout << "printInput - prints the input table" << std::endl;
            std::cout << "printOutput - prints the output table" << std::endl;
            std::cout << "printTypes - prints the types of the cells" << std::endl;
            std::cout << "print - prints all 3 tables" << std::endl;
            std::cout << "setcell - sets a cell value" << std::endl;
            std::cout << "addrow - adds a row full of null values to the bottom" << std::endl;
            std::cout << "addcol - adds a column full of null values to the right" << std::endl;
            std::cout << "reset - resets the table(sets it to 1 by 1 table with a null entry)" << std::endl;
            std::cout << "help - prints this message" << std::endl;
        }
        else
        {
            std::cout << "Invalid command!" << std::endl;
        }
    }
}