#ifndef MasterProcess_HPP
# define MasterProcess_HPP

# include <iostream>
# include <vector>
# include "ListenSocket.hpp"
# include "Multiplexer.hpp"
# include "../ConfigParsing/ServerConfig.hpp"

class MasterProcess {
	private:
		std::vector<ServerConfig> config;
        std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
	public:
        MasterProcess();
		MasterProcess(const std::vector<ServerConfig>& obj);
		void SocketBuilder();
		std::vector<ListenSocket> &getlsocs();
        ~MasterProcess();
};

#endif