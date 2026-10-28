#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include <iostream>
# include "ListenSocket.hpp"
# include "Multiplexer.hpp"

class EventLoop {
    private:
        std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
        Multiplexer EventManager;
    public:
        EventLoop();
        void Socketsbuilder(); //set up listening sockets to create a steady listening connection to the port
        ~EventLoop();
};

#endif