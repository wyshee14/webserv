#ifndef MasterProcess_HPP
# define MasterProcess_HPP

# include <iostream>
# include <vector>
# include <exception>
# include <errno.h>
# include <cstring>
# include <cstdlib>
# include "ListenSocket.hpp"
# include "Multiplexer.hpp"
# include "EventException.hpp"
# include "../ConfigParsing/ServerConfig.hpp"

class MasterProcess : public EventException{
	private:
		std::vector<ServerConfig> config;
        std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
	public:
        MasterProcess();
		MasterProcess(const std::vector<ServerConfig>& obj);
		void errorCloseSockets();
		int SocketBuilder();
		std::vector<ListenSocket> &getlsocs();
        ~MasterProcess();
};

#endif