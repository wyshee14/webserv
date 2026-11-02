#include "ListenSocket.hpp"
#include <errno.h>

ListenSocket::ListenSocket(int Port, std::string IPaddr) : portNum(Port), IPAddr(IPaddr), lsfd(0) {
	std::cout << "ListenSocket Object created" << std::endl;
	std::cout << "ListenSocket address: " << IPAddr << std::endl;
}

ListenSocket::ListenSocket(const ListenSocket& object) {
	
	std::cout << "ListenSocket copy constructor called" << std::endl;
	this->portNum = object.portNum;
	this->sockInfo = object.sockInfo;
	this->IPAddr = object.IPAddr;
	this->lsfd = object.lsfd;
}

void ListenSocket::socketSetup() {
	this->lsfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); //create a socket

	std::cout << "socket successfully created" << std::endl;

	if(this->lsfd >= 0)
	{
		std::cout << portNum << std::endl;
		std::cout << IPAddr << std::endl;
		sockInfo.sin_family = AF_INET; // what family type of data is using (IPV4)
		sockInfo.sin_port = htons(portNum); // port number (htons to convert byte order from network (big endian) to host (little/big endian))

		addrinfo hints;
		addrinfo *res;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		std::cout << "buffering..." << std::endl;
		int stats = getaddrinfo(IPAddr.c_str(), NULL, &hints, &res);
		if (stats != 0)
		{
			std::cerr << "getaddrinfo error: " << gai_strerror(stats) << std::endl;
			close(this->lsfd);
			return;
		}
		sockaddr_in *IPV4 = (sockaddr_in *)res->ai_addr;
		sockInfo.sin_addr.s_addr = IPV4->sin_addr.s_addr; //ip address

		std::cout << sockInfo.sin_addr.s_addr << std::endl;
		int sts = bind(this->lsfd, reinterpret_cast<sockaddr*> (&sockInfo), sizeof(sockInfo)); //binds it to a port
		if (sts != 0)
		{
			int save_errno = errno;
			std::cerr << "bind error: " << strerror(save_errno) << std::endl;
			close(this->lsfd);
			return;
		}
		
		freeaddrinfo(res);
		std::cout << "socket bind successful" << std::endl;

		int status = listen(this->lsfd, SOMAXCONN);
		if (status == 0)
		{
			std::cout << "socket listen successful" << std::endl;
			return;
		}
	}
	return;
}

int ListenSocket::GetPortNum() {
	return (this->portNum);
}

int ListenSocket::Getlsfd() {
	return (this->lsfd);
}

ListenSocket::~ListenSocket() {
	close (this->lsfd);
	std::cout << "ListenSocket Object destroyed" << std::endl;
}

// int main(void)
// {
// 	int status = 0;
// 	ListenSocket client1(8000);
// 	while (!status)
// 	{
// 		status = client1.socketSetup();
// 	}
// 	switch (status) {
// 		case 1:
// 			std::cerr << "Socket bind failure" << std::endl;
// 			break;
// 		case 2:
// 			std::cerr << "Accept client socket failed\n";
// 			break;
// 		case 25:
// 			std::cerr << "Listening socket creation failure" << std::endl;
// 			break;
// 		case 50:
// 			std::cerr << "Listen socket listen failure" << std::endl;
// 			break;
// 	}
// 	return 0;
// }