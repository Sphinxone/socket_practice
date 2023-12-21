#include "Socket.h"
#include <sys/socket.h>
#include <fcntl.h>
Socket::Socket() : fd(-1)
{
}

Socket::Socket(int af, int sock)
{
    fd = socket(af, sock,0);
}

Socket::Socket(int _fd)
{
    fd = _fd;
}

Socket::~Socket()
{
}

void Socket::bind(Address *address)
{
    ::bind(fd,(struct sockaddr *)&address->address,address->socklen);
}

void Socket::listen()
{
    ::listen(fd,5);
}

void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd , F_GETFL) | O_NONBLOCK);
}

int Socket::accept(Address *address)
{
    int connfd = -1;
    connfd = ::accept(fd, (struct sockaddr*) &address->address, &address->socklen);
    return connfd;
}

int Socket::getFd()
{
    return fd;
}
