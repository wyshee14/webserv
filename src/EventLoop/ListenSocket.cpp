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

	this->lsfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); //create a socket
	if(this->lsfd >= 0)
	{
		SockInfo.sin_family = AF_INET; // what family type of data is using (IPV4)
		SockInfo.sin_port = htons(8000); // port number (htons to convert byte order from network (big endian) to host (little/big endian))
		SockInfo.sin_addr.s_addr = INADDR_ANY; //ip address
		if (bind(this->lsfd, reinterpret_cast<sockaddr*> (&SockInfo), sizeof(SockInfo)) == -1) //binds it to a port
		{
			close(this->lsfd);
			return (1);
		}

		std::cout << "socket bind successful" << std::endl;

		int status = listen(this->lsfd, SOMAXCONN);
		if (status == 0)
		{
			// int num = ListenSocket::Multiplexing(this->lsfd);
			return (0);
		}
		return (50);
	}
	return (25);
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