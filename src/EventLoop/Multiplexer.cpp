#include "Multiplexer.hpp"

int Multiplexer::Multiplexing(int listensocket) {
	struct epoll_event ev, events[MAX_VALUE];
	ev.events = EPOLLIN;
	ev.data.fd = listensocket;
	struct sockaddr_in clientsock;
	int epollfd = epoll_create(MAX_VALUE);
	if (epollfd == -1)
	{
		std::cerr << "epoll creation failed\n";
		return (-1);
	}

	std::cout << "epoll creation successful\n";

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listensocket, &ev) == -1)
	{
		std::cerr << "epoll add failed\n";
		return (-1);
	}

	std::cout << "listen socket added to epoll succeed\n";

	while (1)
	{
		int nfds = epoll_wait(epollfd, events, 10, -1);
		if (nfds == -1)
		{
			std::cerr << "epoll wait failure\n";
			return (-1);
		}
		for (int n = 0; n < nfds; n++)
		{
			if (events[n].data.fd == listensocket) //map to the right listensocket immediately
			{
				socklen_t length =  sizeof(clientsock);
				int connec = accept(events[n].data.fd, reinterpret_cast<sockaddr *>(&clientsock), &length);
				if (connec < 0)
				{
					close(listensocket);
					return (2);
				}
				ev.data.fd = connec;
				ev.events = EPOLLIN;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connec, &ev) == -1)
				{
					std::cerr << "epoll add failed\n";
					return (-1);
				}
				else
				{
					std::cout << "client socket successfully added to epoll\n";
					char buffer;
					while(read(connec, &buffer, 1) != 0)
					{
						write(STDOUT_FILENO, &buffer, 1);
					}
					return (100);
				}
			}
		}
	}
}