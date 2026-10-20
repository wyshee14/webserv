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

	this->fd = socket(AF_INET, SOCK_STREAM, 0); //create a socket
	if(fd >= 0)
	{
		sockaddr_in eventinfo; // a struct to store the information of the port setup, bind function required it
		eventinfo.sin_family = AF_INET; // what family type of data is using (IPV4)
		eventinfo.sin_port = htons(8000); // port number (htons to convert byte order from network (big endian) to host (little/big endian))
		eventinfo.sin_addr.s_addr = INADDR_ANY; //ip address
		if (bind(this->fd, reinterpret_cast<sockaddr*> (&eventinfo), sizeof(eventinfo)) == -1) //binds it to a port
		{
			close(fd);
			return (1);
		}

		std::cout << "socket bind successful" << std::endl;

		int status = listen(fd, SOMAXCONN);
		if (status == 0)
		{
			sockaddr_in clientsock;
			socklen_t length = sizeof(eventinfo);
			int clientfd = accept(this->fd, reinterpret_cast<sockaddr*>(&clientsock), &length);
			if (clientfd < 0)
			{
				close(fd);
				return (2);
			}
			else
			{
				std::cout << "client request successfully accepted" << std::endl;
				char buffer;
				while(read(clientfd, &buffer, 1) != 0)
				{
					write(STDOUT_FILENO, &buffer, 1);
				}
				return(100);
			}
		}
		return (50);
	}
	return (25);
}

int ListenSocket::Multiplexing() {
	int epollfd = epoll_create(10);
	if (epollfd == -1)
	{
		std::cerr << "epoll creation failed\n";
		return (1);
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, );
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