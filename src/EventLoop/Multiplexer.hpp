#ifndef MULTIPLEXER_HPP
# define MULTIPLEXER_HPP

# include <iostream>
# include <vector>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h>

#define MAX_VALUE 10

class Multiplexer {
    private:
        int                         epollfd;
        epoll_event                 ev;
        epoll_event                 events[MAX_VALUE];
        std::vector<int>            clientfds; //map container to map clientfd to sockaddr_in struct ?
    public:
        Multiplexer();
        int Multiplexing(int listensocket);
        ~Multiplexer();
};

#endif