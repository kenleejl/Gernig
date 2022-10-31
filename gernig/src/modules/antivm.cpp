/* 

implement the following features:
- registry key checks
- CPUID value checks
- MAC address checks
- running process checks
- running service checks
- file checks

*/

/*
implement MAC address checks by checking OUI of MAC addresses present on the system
00:05:69 (Vmware)
00:0C:29 (Vmware)
00:1C:14 (Vmware)
00:50:56 (Vmware)
00:15:5d (Hyper-V)
08:00:27 (VirtualBox)
52:54:00 (VirtualBox)
00:21:F6 (VirtualBox)
00:14:4F (VirtualBox)
00:0F:4B (VirtualBox)
00:1C:42 (Parallels)
*/

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <macblacklist.h>
#include <iostream>

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


void anti_vm(){
    mac_addr_checks();
}
