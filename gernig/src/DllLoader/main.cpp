#include <string>
#include <iostream>
#include <thread>

#include <windows.h>
#include <loader.hpp>

#ifndef _GERNIG_DEBUG
#include <binexp.h>
#include <defines.h>
#endif

#include <modules/print.hpp>

int main(int argc, char **argv)
{
#ifdef _PRINT_NOISE_ENABLED
    std::thread t1(printLoop, _PRINT_NOISE_TEXT);
#endif

#ifndef _GERNIG_DEBUG
    LoadFromMemory(&BINARY_ARRAY, sizeof(BINARY_ARRAY));
#else
    LoadFromFile(argv[1]);
#endif

    return 0;
}
