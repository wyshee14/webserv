#include "Multiplexer.hpp"

Multiplexer::Multiplexer(const std::vector<ListenSocket>&  ltsocks) {
	for (size_t i; i < ltsocks.size(); i++)
	{
		listSocks[ltsocks[i].Getlsfd()];
	}
	std::cout << "Multiplexer object created" << std::endl; 
}

void Multiplexer::epollCreation() {
	int epollfd = epoll_create(MAX_VALUE);
	if (epollfd == -1)
	{
		std::cerr << "epoll creation failed\n";
		return;
	}

	std::cout << "epoll creation successful\n";
}

void Multiplexer::addlsToEpoll() {
	std::map<int, std::vector<int>>::iterator it;
	for (it = listSocks.begin(); it != listSocks.end(); it++)
	{
		int listensocket = it->first;
		ev.data.fd = listensocket;
		ev.events = EPOLLIN;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listensocket, &ev) == -1)
			{
				std::cerr << "epoll add failed\n";
				return;
			}
		std::cout << " listen socket added to epoll succeed\n";
	}
}

void Multiplexer::fdManagement() {
	while (1)
	{
		int nfds = epoll_wait(epollfd, events, 10, -1);
		if (nfds == -1)
		{
			std::cerr << "epoll wait failure\n";
			return;
		}
		for (int n = 0; n < nfds; n++)
		{
			int ready_fd = events[n].data.fd;
			std::map<int, std::vector<int>>::iterator it = listSocks.find(ready_fd);
			if (it != listSocks.end())
			{
				sockaddr_in clientsock;
				socklen_t length =  sizeof(clientsock);
				int connec = accept(ready_fd, reinterpret_cast<sockaddr *>(&clientsock), &length);
				if (connec < 0)
				{
					return;
				}
				ev.data.fd = connec;
				ev.events = EPOLLIN|EPOLLOUT;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connec, &ev) == -1)
				{
					std::cerr << "epoll add failed\n";
					return;
				}
				else
				{
					it->second.push_back(connec);
					std::cout << "client socket successfully added to epoll\n";
					return;
				}
			}
			else
			{
				//!handles client session's data
			}
		}
	}
}

Multiplexer::~Multiplexer() {
	std::cout << "Multiplexer object destroyed" << std::endl;
}