#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP

# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <cstdlib>
# include <unistd.h>
# include <cstring>

class ListenSocket {
	private:
		int			portNum;
		std::string	IPAddr;
		sockaddr_in	sockInfo;
		int			lsfd;
		
	public:
		ListenSocket(int Port, std::string IPAddr);
		ListenSocket(const ListenSocket& object);
		void socketSetup();
		int GetPortNum() const;
		int Getlsfd() const;
		sockaddr_in GetSockInfo() const;
		~ListenSocket();
};

#endif