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

#define MAX_VALUE 10

// struct domainfd {
//     std::string domain;
//     int fd;
//     void domainfd(int );
// }

class Multiplexer {
    private:
        //!change keys to a struct or listensocket class
        //!optimize for efficient access to the right clientfd
        //!use port to get the right keys
        //!potentially use binary integer representation of ip to compare
        //!use it to remove value element (clientfd) from key element (listen socket)
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
        ~Multiplexer();
};

#endif