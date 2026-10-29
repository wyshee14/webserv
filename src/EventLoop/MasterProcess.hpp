#ifndef MasterProcess_HPP
# define MasterProcess_HPP

# include <iostream>
# include <vector>
# include "ListenSocket.hpp"
# include "../ConfigParsing/ServerConfig.hpp"

class MasterProcess {
	private:
		int port;
    protected:
        std::vector<ListenSocket> lsocs; //how do you relate listensocket fd to epoll
    public:
        MasterProcess(); //
		void SocketBuilder(size_t quant);
		std::vector<ListenSocket> &getlsocs();
        ~MasterProcess();
};

#endif