#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>

class ListenSocket {
	private:
		int PortNum;
		sockaddr_in	SockInfo;
		int			lsfd;
	public:
		ListenSocket(int Port);
		void socketSetup();
		int GetPortNum();
		int Getlsfd();
		sockaddr_in GetSockInfo();
		~ListenSocket();
};

#endif