/*

    Simple cross-platformed program for IP lookup

    This program takes a name or IP address for its only argument. It then
    uses getaddrinfo() to resolve that name or that IP address into an address structure, and
    the program prints that IP address using getnameinfo() for the text conversion. If
    multiple addresses are associated with a name, it prints each of them. It also indicates any
    errors.

    This program is analogous to one of the examples from the book 
    "Hands-On Network Programming with C", 2019 by Lewis Van Winkle

*/

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif


#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

int main(int argc, char* argv[])
{
    //checking for correct number of comand line arguments
    if (argc < 2) 
    {
        printf("Usage:\n\tlookup hostname\n");
        printf("Example:\n\tlookup example.com\n");
        exit(0);
    }

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) 
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif   

    
    //calling getaddrinfo() to convert the hostname or address into a struct addrinfo
    printf("Resolving hostname '%s'\n", argv[1]);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ALL;
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) 
    {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    
    //converting the addresses to text with getnameinfo() function 
    //and printing them to the screan
    printf("Remote address is:\n");
    struct addrinfo *address = peer_address;
    do 
    {
        char address_buffer[100];
        getnameinfo(address->ai_addr, address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                0, 0, NI_NUMERICHOST);
        printf("\t%s\n", address_buffer);
    } while ((address = address->ai_next));


    freeaddrinfo(peer_address);

#if defined(_WIN32)
    WSACleanup();
#endif    


    return 0;
}