/*

    This program lists local addresses of network adapters for Unix-based systems

    This program is analogous to one of the examples from the book 
    "Hands-On Network Programming with C", 2019 by Lewis Van Winkle

*/

#include<sys/socket.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
    //struct pointer to a structure which stores the addresses of network adaptors
    struct ifaddrs *adapterAddresses;

    //the getifaddrs function allocates memory and fills in a linked list of addresses
    //this function returns 0 on success or -1 on failure
    if (getifaddrs(&adapterAddresses) == -1) 
    {
        printf("getifaddrs call failed\n");
        return -1;
    }

    //pointer to walk through the linked list of addresses
    struct ifaddrs *address = adapterAddresses;

    //loop stops when address == 0, which happens at the end of the linked list
    while (address)
    {
        if (address->ifa_addr == NULL) 
        { 
            address = address->ifa_next;
            continue;
        }

        int family = address->ifa_addr->sa_family;

        //we only consider IPv4 or IPv6 types, although the getifaddrs function can detect other types
        if (family == AF_INET || family == AF_INET6)
        {
            
            //print adapter name
            printf("%s\t", address->ifa_name);

            //print address type (IPv4 or IPv6)
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");

            //defune a buffer to store the textual address of the adapter
            char text_address[100];


            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            
            //this function fills in the text_address buffer
            getnameinfo(address->ifa_addr, family_size, text_address, sizeof(text_address), 0, 0, NI_NUMERICHOST);

            //print textual address of the adapter
            printf("\t%s\n", text_address);

        }

        //walk through the structure
        address = address->ifa_next;
    }

    //free the memory allocated by getifaddrs
    freeifaddrs(adapterAddresses);
    return 0;
}