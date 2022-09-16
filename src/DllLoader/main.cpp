#include <string>
#include <iostream>
#include <thread>

#include <windows.h>
#include <loader.hpp>

#include <modules/print.hpp>

int main(int argc, char **argv)
{
    std::thread t1(printLoop, "Hello");
    LoadFromFile(argv[1]);
    t1.join();

    return 0;
}
