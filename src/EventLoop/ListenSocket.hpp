#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include <iostream>
#include <vector>

class ListenSocket {
	private:
		sockaddr_in	SockInfo;
		int			lsfd;
	public:
		ListenSocket();
		int socketSetup();
		int Getlsfd();
		sockaddr_in GetSockInfo();
		~ListenSocket();
};

#endif