#ifndef MULTIPLEXER_HPP
# define MULTIPLEXER_HPP

# include <iostream>
# include <vector>
# include <map>
#include <algorithm>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h>
# include <fcntl.h>
# include "ListenSocket.hpp"
# include "EventException.hpp"

#define MAX_VALUE 10

class Multiplexer {
    private:
        std::map<int, std::vector<int> >   listSocks;
        std::map<int, int>                 clientToListen;
        int                                epollfd;
        epoll_event                        ev;
        epoll_event                        events[MAX_VALUE];
    public:
        Multiplexer(const std::vector<ListenSocket>& ltsocks);
        void epollCreation();
        void addlsToEpoll();
        void eventLoop();
        virtual ~Multiplexer();
};

#endif