#include <string>
#include <iostream>
#include <thread>

#include <windows.h>
#include <loader.hpp>

void task1(std::string msg)
{
    for (int i = 0; i < 500; i++)
    {
        std::cout << "task1: " << msg << std::endl;
        Sleep(1000);
    }
}

int main(int argc, char **argv)
{
    std::thread t1(task1, "Hello");
    LoadFromMemory(argv[1]);
    t1.join();

    return 0;
}
