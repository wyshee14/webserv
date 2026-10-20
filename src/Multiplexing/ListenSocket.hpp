#ifndef MULTIPLEXER
#define MULTIPLEXER

#include <iostream>
#include <vector>

class ListenSocket {
	private:
		std::vector<int> fds;
		int fd;
	public:
		ListenSocket();
		int socketSetup();
		int Multiplexing();
		~ListenSocket();
		// std::vector<int> getFds();
};

#endif