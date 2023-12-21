#pragma once
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
class Address
{
    // private:
    //     struct sockaddr_in address;
    //     socklen_t socklen;
    public:
        struct sockaddr_in address;
        socklen_t socklen;
        Address();
        Address(int af, char *ip, int port);
        ~Address();
        // sockaddr_in getAddress(void);
};

// Address::Address(int af,char* ip,int port){
//     memset(&address, 0,sizeof(address));
//     address.sin_family = af;
//     inet_pton(af,ip,&address.sin_addr);
//     address.sin_port = htons(port);
// }

// sockaddr_in Address::getAddress(){
//     return address;
// }

