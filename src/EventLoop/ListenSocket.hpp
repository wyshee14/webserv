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
# include "EventException.hpp"

class ListenSocket : public EventException{
	private:
		bool		_pass;
		int			_portNum;
		std::string	_IPAddr;
		sockaddr_in	_sockInfo;
		int			_lsfd;
		
	public:
		ListenSocket(int Port, std::string IPAddr);
		ListenSocket(const ListenSocket& object);
		void socketSetup();
		int GetPass() const;
		void SetPass(bool flag);
		int GetPortNum() const;
		int Getlsfd() const;
		sockaddr_in GetSockInfo() const;
		~ListenSocket();
};

#endif