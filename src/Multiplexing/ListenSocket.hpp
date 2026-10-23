#ifndef MULTIPLEXER
#define MULTIPLEXER

#include <iostream>
#include <vector>

#define MAX_VALUE 10

class ListenSocket {
	private:
		std::vector<int> fds;
		int fd;
	public:
		ListenSocket();
		int socketSetup();
		int Multiplexing(int listensocket);
		~ListenSocket();
		// std::vector<int> getFds();
};

#endif