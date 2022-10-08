/*
- create a custom struct to hold these domain names
- get both valid and invalid domain names
- insert them into the code encrypted and query these domains on runtime
- hash the domains with the expected results and insert them together with the code
- check the reply to the queries by hashing it with the respective domains and comparing the hardcoded hashes 
*/

#undef UNICODE
// #include <winsock2.h>
// #include <ws2tcpip.h>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <dnsnoise.h>
#include <dnsanalysis.h>
#include <time.h>

// link with Ws2_32.lib
// #pragma comment (lib, "Ws2_32.lib")

#include <modules/dns.hpp>
// #include <cpr/cpr.h>
// #include <nlohmann/json.hpp>
// using namespace std;
// using json = nlohmann::json;

/*
bool queryDomainName(std::string arg){

    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    INT iRetval;

    DWORD dwRetval;

    int i = 1;
    
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;

    struct sockaddr_in  *sockaddr_ipv4;
    //    struct sockaddr_in6 *sockaddr_ipv6;
    LPSOCKADDR sockaddr_ip;

    char ipstringbuffer[46];
    DWORD ipbufferlength = 46;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    char * domain_name;
    domain_name = &arg[0];

    //--------------------------------
    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("Calling getaddrinfo with following parameters:\n");
    printf("\tnodename = %s\n", domain_name);
    printf("\tservname (or port) = %s\n\n", "0");
    
    //--------------------------------
    // Call getaddrinfo(). If the call succeeds,
    // the result variable will hold a linked list
    // of addrinfo structures containing response
    // information
    dwRetval = getaddrinfo(domain_name, "0", &hints, &result);
    if ( dwRetval != 0 ) {
        printf("getaddrinfo failed with error: %d\n", dwRetval);
        WSACleanup();
        return false;
    }
    
    printf("getaddrinfo returned success\n");
    
    // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        printf("getaddrinfo response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
        printf("\tFamily: ");
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                printf("\tIPv4 address %s\n",
                    inet_ntoa(sockaddr_ipv4->sin_addr) );
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                // the InetNtop function is available on Windows Vista and later
                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                // printf("\tIPv6 address %s\n",
                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
                
                // We use WSAAddressToString since it is supported on Windows XP and later
                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                ipbufferlength = 46;
                iRetval = WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, 
                    ipstringbuffer, &ipbufferlength );
                if (iRetval)
                    printf("WSAAddressToString failed with %u\n", WSAGetLastError() );
                else    
                    printf("\tIPv6 address %s\n", ipstringbuffer);
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
        printf("\tSocket type: ");
        switch (ptr->ai_socktype) {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol) {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }
    
    freeaddrinfo(result);
    WSACleanup();

    return true;
}
*/

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
    
    // std::vector<std::string> real_dns;
    REAL_DNS.push_back("reddit.com");
    REAL_DNS.push_back("google.com");

    // std::vector<std::string> fake_dns;
    FAKE_DNS.push_back("flightterrible.bn");
    FAKE_DNS.push_back("harshthreatening.wf");
    for (auto it = REAL_DNS.begin(); it != REAL_DNS.end(); ++it){
        if (getHostByName(*it) == false ){
            printf("unable to resolve domain name. quitting...");
            exit(0);
        }
        Sleep(1000);
    }
    printf("Finished checking for real dns resolvers.");

    for (auto it = FAKE_DNS.begin(); it != FAKE_DNS.end(); ++it){
        if (getHostByName(*it) == true ){
            printf("fake resolver detected!!! quitting...");
            exit(0);
        }
        Sleep(1000);
    }
    printf("Finished checking for fake dns resolvers.");

    // while (true){
    //     if (getHostByName("reddit.com") == false ){
    //         printf("unable to resolve domain name. quitting...");
    //         exit(0);
    //     }
    //     if (getHostByName("google.com") == false ){
    //         printf("unable to resolve domain name. quitting...");
    //         exit(0);
    //     }
    //     Sleep(1000);
    // }

    // json dns_json = json::parse(arg);

    // json dns = {
    //     {"reddit.com", true},
    //     {"fuzzybomb.td", false}
    // };

    /*
    
    

    

    
    */
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
/*
int main(){
    // json json_str = json::parse(R"({
    //     "capriciouslike.ws": true,
    //     "flightterrible.bn": false
    // })");
    checkDNS(NULL);
}*/