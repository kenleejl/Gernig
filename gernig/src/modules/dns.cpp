// #include <winsock2.h>
// #include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <windows.h>

#include <modules/dns.hpp>

void getHostByName(std::string arg)
{
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }
    while (1)
    {

        DWORD dwError;
        int i = 0;

        struct hostent *remoteHost;
        const char *host_name;
        struct in_addr addr;

        char **pAlias;

        host_name = arg.c_str();

        printf("Calling gethostbyname with %s\n", host_name);
        remoteHost = gethostbyname(host_name);

        if (remoteHost == NULL)
        {
            dwError = WSAGetLastError();
            if (dwError != 0)
            {
                if (dwError == WSAHOST_NOT_FOUND)
                {
                    printf("Host not found\n");
                    return;
                }
                else if (dwError == WSANO_DATA)
                {
                    printf("No data record found\n");
                    return;
                }
                else
                {
                    printf("Function failed with error: %ld\n", dwError);
                    return;
                }
            }
        }
        else
        {
            printf("Function returned:\n");
            printf("\tOfficial name: %s\n", remoteHost->h_name);
            for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++)
            {
                printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
            }
            printf("\tAddress type: ");
            switch (remoteHost->h_addrtype)
            {
            case AF_INET:
                printf("AF_INET\n");
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS\n");
                break;
            default:
                printf(" %d\n", remoteHost->h_addrtype);
                break;
            }
            printf("\tAddress length: %d\n", remoteHost->h_length);

            i = 0;
            if (remoteHost->h_addrtype == AF_INET)
            {
                while (remoteHost->h_addr_list[i] != 0)
                {
                    addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
                    printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
                }
            }
            else if (remoteHost->h_addrtype == AF_NETBIOS)
            {
                printf("NETBIOS address was returned\n");
            }
        }
        Sleep(1000);
    }
}