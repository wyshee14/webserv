#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

# include <iostream>
# include "ListenSocket.hpp"
# include "Multiplexer.hpp"
# include "MasterProcess.hpp"

class EventLoop : public MasterProcess {
    private:
        // std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
        Multiplexer EventManager;
    public:
        EventLoop();
		void CoreMechanic();
        ~EventLoop();
};

#endif