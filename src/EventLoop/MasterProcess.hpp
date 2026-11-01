#ifndef MasterProcess_HPP
# define MasterProcess_HPP

# include <iostream>
# include <vector>
# include "ListenSocket.hpp"
# include "../ConfigParsing/ServerConfig.hpp"

class MasterProcess {
	private:
		std::vector<ListenDirective> config;
    protected:
        std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
    public:
        MasterProcess();
		MasterProcess(const std::vector<ServerConfig>& obj);
		void ParseEndpoint(); // write a double for loop to parse listen endpoint
		void SocketBuilder(size_t quant);
		std::vector<ListenSocket> &getlsocs();
        ~MasterProcess();
};

#endif