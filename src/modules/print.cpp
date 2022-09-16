#include <iostream>
#include <windows.h>
#include <modules/print.hpp>

void printLoop(std::string msg)
{
    for (int i = 0; i < 500; i++)
    {
        std::cout << "printLoop " << i << ": " << msg << std::endl;
        Sleep(1000);
    }
}
