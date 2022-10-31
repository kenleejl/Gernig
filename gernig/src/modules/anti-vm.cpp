#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <modules/anti-vm.hpp>

bool asm_detection () {
	CONTEXT Ctx = { 0 };
	HANDLE hHandle = GetCurrentThread();

	Ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	if (GetThreadContext(hHandle, &Ctx)) {
		if ((Ctx.Dr0 != 0x00) || (Ctx.Dr1 != 0x00) || (Ctx.Dr2 != 0x00) || (Ctx.Dr3 != 0x00) || (Ctx.Dr6 != 0x00) || (Ctx.Dr7 != 0x00)) {
			return true;
			exit(0x2000); //Exceeds CPU RANGE
	}

	return false;

	}
	return true;
}


//https://github.com/TheDuchy/rdtsc-cpuid-vm-check/blob/master/main.c
#ifdef _WIN32 
    #include <intrin.h>
    #pragma intrinsic(__rdtsc)
#endif

bool rdtsc_cpuid_check(){
    unsigned long long int time1, time2, sum = 0;
    const unsigned char avg = 100;
    
    for (int i = 0; i < avg; i++){
        time1 = __rdtsc();
#ifdef _WIN32
        __asm ("cpuid");
#endif
        time2 = __rdtsc();
        sum += time2 - time1;
    }

    sum = sum / avg;
    
    printf("Ticks on average: %llu\n", sum);

    if(sum > 500){
		// VM
        return true;
    }else{
		// Bare Metal
        return false;
    }
}



#include <tchar.h>
#include <psapi.h>
#include <processanalysis.h>

// modified sample code from https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1x
void process_checks( )
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        exit(1);
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

			// Get a handle to the process.

			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
										PROCESS_VM_READ,
										FALSE, aProcesses[i] );

			// Get the process name.

			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
					&cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, 
									sizeof(szProcessName)/sizeof(TCHAR) );
				}
			}

			// Print the process name and identifier.

			if (_tcscmp(szProcessName, TEXT("<unknown>"))){
				for (auto it = PROCESS_LIST.begin(); it != PROCESS_LIST.end(); ++it){
					std::string str = *it;
					TCHAR *proc_name = new TCHAR[str.size()+1];
					proc_name[str.size()] = 0;
					std::copy(str.begin(), str.end(), proc_name);
					// _tprintf( TEXT("%s | %s \n"), szProcessName, proc_name );
					if (_tcscmp(szProcessName, proc_name) == 0){
						// _tprintf(TEXT("current process name: %s\n"), szProcessName);
						// _tprintf(TEXT("blacklisted match: %s\n"), proc_name);
						abort();
					}
				}
			}

			// Release the handle to the process.

			CloseHandle( hProcess );
        }
    }
}

void sleep_check(int sleep_time){
	DWORD sleep_start = GetTickCount();
	Sleep(sleep_time);
	DWORD sleep_stop = GetTickCount();
	// std::cout << "sleep_start: " << sleep_start << std::endl;
	// std::cout << "sleep_stop: " << sleep_stop << std::endl;
	// std::cout << "time difference: " << sleep_stop - sleep_start << std::endl;
	// std::cout << "sleep_time: " << sleep_time << std::endl;
	if (sleep_time - 100 < sleep_time - sleep_start > sleep_time + 100){
		abort();
	}
}


/*
implement MAC address checks by checking OUI of MAC addresses present on the system
*/

#include <iphlpapi.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <macblacklist.h>
#include <modules/anti-vm.hpp>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

void mac_addr_checks()
{
    // modified sample code from https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
    /* Declare and initialize variables */

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) {
            exit(1);
        }
        dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        } else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        int i = 0;
        int x = 0;
        while (pCurrAddresses) {
            if (pCurrAddresses->PhysicalAddressLength != 0) {
                x++;
                char buf[9] = {'\0'};
                sprintf(buf, "%.2X:%.2X:%.2X", (int) pCurrAddresses->PhysicalAddress[0], (int) pCurrAddresses->PhysicalAddress[1], (int) pCurrAddresses->PhysicalAddress[2]);
                for (auto it = MAC_BLACKLIST.begin(); it != MAC_BLACKLIST.end(); ++it){                 
                    if ((buf == *it)){
                        i++;
                    }
                }
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
        if (i == x){
            exit(1);
        }
    } else {
        if (dwRetVal != ERROR_NO_DATA) {
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                    NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   
                    // Default language
                    (LPTSTR) & lpMsgBuf, 0, NULL)) {
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    FREE(pAddresses);
                exit(1);
            }
        }
    }

    if (pAddresses) {
        FREE(pAddresses);
    }
}