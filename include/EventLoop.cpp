#include "EventLoop.h"
#include <vector>
#include "Channel.h"
#include "Epoll.h"
EventLoop::EventLoop()
{
    ep = new Epoll(5);
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel *> channels = ep->poll();
        for (auto it = channels.begin(); it != channels.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}
