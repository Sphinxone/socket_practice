#include "Epoll.h"
#include <sys/epoll.h>
#include <string.h>
#include "Channel.h"
Epoll::Epoll(int number = 1)
{
    epollfd = epoll_create(number);
    epoll_event events[MAX_EVENT_NUMBER];
    memset(events,0,MAX_EVENT_NUMBER);
}
Epoll::Epoll()
{
    epollfd = epoll_create(1);
    epoll_event events[MAX_EVENT_NUMBER];
    memset(events,0,MAX_EVENT_NUMBER);
}

Epoll::~Epoll()
{
}

void Epoll::addfd(int fd, bool enable_et)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(enable_et){
        ev.events |= EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

void Epoll::updateChannel(Channel * channel)
{
    int fd = channel->getFd();
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    //这个epoll_vent真的太精妙了，ev.data.prt是空指针
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        epoll_ctl(epollfd,EPOLL_CTL_ADD,fd, &ev);
        channel->setInEpoll();
    }else{
        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
    }
}

std::vector<Channel *> Epoll::poll(int timeout)
{
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epollfd,events,MAX_EVENT_NUMBER-1,timeout);
    for(int i=0;i<nfds;++i){
        //太过于精妙了 ptr是空指针
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
    // return std::vector<epoll_event>();
}

