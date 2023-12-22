# 整体流程

`EventLoop` 将循环获取就绪`Channel`，并调用`Channel`的回调函数进行处理。

`Server` 管理服务器上连接，一般分为两种一种是建立连接`Acceptor`，一种是连接`Connection`。在`Server`中处理`Acceptor`和`Connection`相关的逻辑。

`Epoll`主要是对于epoll做更高一层的封装，包括添加，事件监听。

`Acceptor`管理连接请求，在其中进行socket绑定并进行监听，在回调函数中对于新接受连接进行绑定。

`Connection`对于连接事件进行处理，包括接收数据还有断开连接。

`Channel`对于连接描述符进行更为细粒度的描述，主要表现为存储更为细致的信息包括events，revents，回调函数等

`Socket`主要是管理连接描述符相关的函数，比如bind，listen，accept

`Address`主要是与地址相关的类，包括sockaddr和socklen

# EventLoop

EventLoop作为轮询事件的类，最主要的是包含Epoll，并实现loop函数。

    Epoll *epfd;
    bool stop;

# Server
Server的作用是管理服务器整个周期，使用Reactor异步服务器模型，因此需要对于接收连接和连接进行管理。

    Acceptoor *acceptor;
    std::map<int,Connection> connections;
    Eventloop *loop //这里面使用Epoll *epfd也是可以的，但是可能是为了进一步解耦，就使用了*loop
    

# Epoll
Epoll主要是对于Epoll进行进一步的封装，与Chanel相结合。

    int epfd;//epoll描述符
    epoll_event* events;
    
# Acceptor
管理连接请求，主要连接成功之后需要建立连接，连接之前需要监听端口。

    EventLoop *loop;//需要注册连接
    Socket *sock;//包括bind，accept
    Channel *acceptChannel;
    std::function<void()> cb;
    Address *address;

# Connection
连接相关

    EventLoop *loop;
    Socket *sock;
    Channel *connectChannel;
    std::function<void()> cb;


# Channel
有一些变量看似不重要，但其实是可以层层传递到真正使用这个这个变量的类上，比如loop

    int fd;
    EventLoop *loop;//其中包含Epoll，用于注册事件
    boll inEpoll;//是否已经被注册到了Epoll中
    uint32_t events;
    uint32_t revents;
    std::function<void()> cd;

# Socket

    int fd


# Address
Address主要用在bind accept上，bind是Server端控制的，因此传入的Address为Server自行设定的，而accept端是接收Client的连接，因此传入的Address是指针，用于接收Client端的连接请求。

    struct sockaddr_in address;
    socklen_t socklen;


# 所用技术解析

## 上锁

std::unique_lock VS std::lock_guard

std::lock_guard是使用RALL语法的模板类，std::unique_lock是更加灵活，可以显式调用lock和unlock