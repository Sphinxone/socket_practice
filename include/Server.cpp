#include "Server.h"
#include "Acceptor.h"
#include "Address.h"
#include "Socket.h"
#include "Channel.h"
#include <string.h>
#include <unistd.h>
#include <cerrno>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop)
{
    loop = _loop;
    Address *address = new Address(AF_INET,"127.0.0.1",8090);
    acceptor = new Acceptor(loop, address);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnetion,this,std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while(true){
        memset(&buf, '\0', READ_BUFFER);
        ssize_t bytes_read = read(sockfd,buf,sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s \n",sockfd,buf);
            write(sockfd,buf,sizeof(buf));
        }else if(bytes_read == -1 && errno == EINTR){
            printf("continue reading");
            continue;
        }else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            printf("finish reading once, errno: %n\n", errno);
            break;
        }else if(bytes_read == 0){
            printf("EOF, client fd %d disconnected \n",sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::newConnetion(Socket *serv_sock)
{
    Address *clnt_addr = new Address();
    Socket *clnt_sock = new Socket(clnt_sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port%d\n", clnt_sock->getFd(),inet_ntoa(clnt_addr->address.sin_addr),ntohs(clnt_addr->address.sin_port));
    clnt_sock->setnonblocking();
    Channel *clnt_channel = new Channel(loop,clnt_sock->getFd());
    std::function<void()> _cb = std::bind(Server::handleReadEvent,this,clnt_sock->getFd());
    clnt_channel->setCallback(_cb);
    clnt_channel->enableReading();
}

void Server::deleteConnection(Socket *sock)
{
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}
