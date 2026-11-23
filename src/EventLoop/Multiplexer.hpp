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
# include "Client.hpp"

# define MAX_VALUE 10
# define BUFFERSIZE 4096

class Multiplexer {
	private:
		std::map<int, std::vector<int> >	listSocks;
		std::map<int, int>					clientToListen;
		std::map<int, Client>				clientFdToObject;
		int									epollfd;
		epoll_event							ev;
		epoll_event							events[MAX_VALUE];
	public:
		Multiplexer(const std::vector<ListenSocket>& ltsocks);
		void epollCreation();
		void addlsToEpoll();
		void eventLoop();
		~Multiplexer();
};

#endif