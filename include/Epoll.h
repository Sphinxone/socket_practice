#pragma once
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENT_NUMBER 1024
class Channel;
class Epoll
{
    private:
        epoll_event* events;
        int epollfd;

    public:
        Epoll(int number = 1);
        Epoll();
        ~Epoll();
        void addfd(int fd, bool enable_et);
        void updateChannel(Channel *);
        std::vector<Channel*> poll(int timeout=-1);
};