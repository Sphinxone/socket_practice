#include "Address.h"

Address::Address()
{
}

Address::Address(int af, char *ip, int port)
{
    address.sin_family = af;
    inet_pton(af, ip, &address.sin_addr);
    address.sin_port = htons(port);
}

Address::~Address()
{
    delete &address;
    delete &socklen;
}
