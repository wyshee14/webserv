#include "ListenSocket.hpp"
#include <errno.h>

ListenSocket::ListenSocket(int Port, std::string IPaddr) : _pass(false), _portNum(Port), _IPAddr(IPaddr), _lsfd(-1) {
	std::cout << "ListenSocket Object created" << std::endl;
	std::cout << "ListenSocket address: " << _IPAddr << std::endl;
}

ListenSocket::ListenSocket(const ListenSocket& object) {
	
	std::cout << "ListenSocket copy constructor called" << std::endl;
	this->_portNum = object._portNum;
	this->_sockInfo = object._sockInfo;
	this->_IPAddr = object._IPAddr;
	this->_lsfd = object._lsfd;
}

void ListenSocket::socketSetup() {
	this->_lsfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); //create a socket

	if(this->_lsfd >= 0)
	{
		std::cout << "socket successfully created" << std::endl;
		std::cout << _portNum << std::endl;
		std::cout << _IPAddr << std::endl;
		_sockInfo.sin_family = AF_INET; // what family type of data is using (IPV4)
		_sockInfo.sin_port = htons(_portNum); // port number (htons to convert byte order from network (big endian) to host (little/big endian))

		addrinfo hints;
		addrinfo *res;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		std::cout << "buffering..." << std::endl;
		int stats = getaddrinfo(_IPAddr.c_str(), NULL, &hints, &res);
		if (stats != 0)
		{
			close(this->_lsfd);
			throw EventException::GETADDRINFOEXCEPTION;
		}
		sockaddr_in *IPV4 = (sockaddr_in *)res->ai_addr;
		_sockInfo.sin_addr.s_addr = IPV4->sin_addr.s_addr; //ip address

		std::cout << _sockInfo.sin_addr.s_addr << std::endl;

		int reuse = 1;
		if (setsockopt(this->_lsfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
		{
			EventException::setSaveErrno(errno);
			close(this->_lsfd);
			throw EventException::SOCKOPTEXCEPTION;
		}

		int sts = bind(this->_lsfd, reinterpret_cast<sockaddr*> (&_sockInfo), sizeof(_sockInfo)); //binds it to a port
		if (sts != 0)
		{
			EventException::setSaveErrno(errno);
			close(this->_lsfd);
			throw EventException::BINDEXCEPTION;
		}
		
		freeaddrinfo(res);
		std::cout << "socket bind successful" << std::endl;

		int status = listen(this->_lsfd, SOMAXCONN);
		if (status < 0)
		{
			EventException::setSaveErrno(errno);
			close(this->_lsfd);
			throw EventException::LISTENEXCEPTION;
		}
		std::cout << "socket listen successful" << std::endl;
		return;
	}
	EventException::setSaveErrno(this->_lsfd);
	throw EventException::SOCKETSETUPEXCEPTION;
}

int ListenSocket::GetPass() const {
	return (this->_pass);
}

void ListenSocket::SetPass(bool flag) {
	_pass = flag;
}

sockaddr_in ListenSocket::GetSockInfo() const {
	return (this->_sockInfo);
}

int ListenSocket::GetPortNum() const {
	return (this->_portNum);
}

int ListenSocket::Getlsfd() const {
	return (this->_lsfd);
}

ListenSocket::~ListenSocket() {
	close (this->_lsfd);
	std::cout << "ListenSocket Object destroyed" << std::endl;
}

// int main(void)_
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