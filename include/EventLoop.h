
class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop(/* args */);
    ~EventLoop();
    void loop();
    void updateChannel(Channel *);
};