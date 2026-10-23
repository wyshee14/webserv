#include "ListenSocket.hpp"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>

ListenSocket::ListenSocket() {
	std::cout << "ListenSocket Object created" << std::endl;
}

int ListenSocket::socketSetup() {

	this->fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); //create a socket
	if(this->fd >= 0)
	{
		sockaddr_in eventinfo; // a struct to store the information of the port setup, bind function required it
		eventinfo.sin_family = AF_INET; // what family type of data is using (IPV4)
		eventinfo.sin_port = htons(8000); // port number (htons to convert byte order from network (big endian) to host (little/big endian))
		eventinfo.sin_addr.s_addr = INADDR_ANY; //ip address
		if (bind(this->fd, reinterpret_cast<sockaddr*> (&eventinfo), sizeof(eventinfo)) == -1) //binds it to a port
		{
			close(this->fd);
			return (1);
		}

		std::cout << "socket bind successful" << std::endl;

		int status = listen(this->fd, SOMAXCONN);
		if (status == 0)
		{
			int num = ListenSocket::Multiplexing(this->fd);
			return (num);
		}
		return (50);
	}
	return (25);
}

int ListenSocket::Multiplexing(int listensocket) {
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
			if (events[n].data.fd == listensocket)
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

ListenSocket::~ListenSocket() {
	std::cout << "ListenSocket Object destroyed" << std::endl;
}

int main(void)
{
	int status = 0;
	ListenSocket client1;
	while (!status)
	{
		status = client1.socketSetup();
	}
	switch (status) {
		case 1:
			std::cerr << "Socket bind failure" << std::endl;
			break;
		case 2:
			std::cerr << "Accept client socket failed\n";
			break;
		case 25:
			std::cerr << "Listening socket creation failure" << std::endl;
			break;
		case 50:
			std::cerr << "Listen socket listen failure" << std::endl;
			break;
	}
	return 0;
}