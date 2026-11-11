#include "Multiplexer.hpp"
#include <errno.h>
#include <vector>
// #include <cstring>

Multiplexer::Multiplexer(const std::vector<ListenSocket>& ltsocks)  : epollfd(-1) {
	std::vector<ListenSocket>::const_iterator it;
	for (it = ltsocks.begin(); it != ltsocks.end(); it++)
	{
		std::cout << it->Getlsfd() << std::endl;
	}
	
	for (size_t i = 0; i < ltsocks.size(); i++)
	{
		listSocks[ltsocks[i].Getlsfd()];
	}
	std::map<int, std::vector<int> >::const_iterator iter;
	for (iter = listSocks.begin(); iter != listSocks.end(); iter++)
	{
		std::cout << iter->first << std::endl;
	}
	std::cout << "Multiplexer object created" << std::endl; 
}

void Multiplexer::epollCreation() {
	this->epollfd = epoll_create(MAX_VALUE);
	if (epollfd == -1)
	{
		std::cerr << "epoll creation failed\n";
		return;
	}

	std::cout << "epoll creation successful\n";
}

void Multiplexer::addlsToEpoll() {
	std::map<int, std::vector<int> >::iterator it;
	for (it = listSocks.begin(); it != listSocks.end(); it++)
	{
		const int &listensocket = it->first;
		ev.data.fd = listensocket;
		ev.events = EPOLLIN|EPOLLET;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listensocket, &ev) == -1)
			{
				int save_errno = errno;
				std::cerr << "epoll add failed: " << strerror(save_errno) << std::endl;
				return;
			}
		std::cout << " listen socket added to epoll succeed\n";
	}
}

int set_nonblocking(int sock_fd) {
	int flag;

	flag = fcntl(sock_fd, F_GETFL, 0);
	if (flag == -1)
	{
		std::cerr << "fcntl get failed" << std::endl;
		return (-1);
	}
	flag |= O_NONBLOCK;

	if (fcntl(sock_fd, F_SETFL, flag) == -1)
	{
		std::cerr << "fcntl set failed" << std::endl;
		return (-1);
	}
	return (sock_fd);
}

void printMapClientfds(std::map<int, std::vector<int> >& list, std::map<int, int >& reverselist)
{
	std::map<int, std::vector<int> >::const_iterator it;
	for (it = list.begin(); it != list.end();  it++)
	{
		std::cout << "key: " << it->first << " value: ";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			std::cout << it->second.at(i) <<  " ";
		}
		std::cout << "\n";
		std::cout << "size: " << it->second.size() << "\n" << std::endl;
	}
	std::map<int, int >::const_iterator iter;
	for (iter = reverselist.begin(); iter != reverselist.end(); iter++)
	{
		std::cout << "key(client): " << iter->first << " --> value(listen): " << iter->second << std::endl;
	}
}

void removeClientfd(int session, std::map<int, std::vector<int> >& list, std::map<int, int >& reverselist)
{
	int listen = reverselist[session];
	std::vector<int>& clients =  list[listen];
	clients.erase(remove(clients.begin(), clients.end(), session), clients.end());
	reverselist.erase(session);
}

void Multiplexer::eventLoop() {
	epollCreation();
	addlsToEpoll();
	while (1)
	{
		std::cout << "i am back up on the while loop" << std::endl;
		int nfds = epoll_wait(epollfd, events, 10, -1);
		if (nfds == -1)
		{
			std::cerr << "epoll wait failure\n";
			return;
		}
		for (int n = 0; n < nfds; n++)
		{
			int ready_fd = events[n].data.fd;
			std::map<int, std::vector<int> >::iterator it = listSocks.find(ready_fd);
			if (it != listSocks.end())
			{
				sockaddr_in clientsock;
				socklen_t length =  sizeof(clientsock);
				while (1)
				{
					int connec = accept(ready_fd, reinterpret_cast<sockaddr *>(&clientsock), &length);
					if (connec == -1 && errno == EAGAIN)
					{
						std::cout << "EAGAIN: breaking out of accept loop" << std::endl;	
						break;
					}
					if (connec == -1)
					{
						std::cout << "accept error: " << errno << std::endl;
						break;
					}
					ev.data.fd = connec;
					ev.events = EPOLLIN|EPOLLET;
					connec = set_nonblocking(connec);
					this->clientToListen[connec] = ready_fd;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connec, &ev) == -1)
					{
						int save_errno = errno;
						std::cerr << "epoll add failed: " << strerror(save_errno) << std::endl;
						return;
					}
					else
					{
						it->second.push_back(connec);
						this->clientToListen[connec] = ready_fd;
						std::cout << "client socket successfully added to epoll\n";
						printMapClientfds(this->listSocks, this->clientToListen);
					}
				}
			}
			else
			{
				std::cout << "processing client session" << std::endl;
				int session = events[n].data.fd;
				sockaddr_in clientinfo;
				socklen_t length = sizeof(clientinfo);
				if (getsockname(session, reinterpret_cast<sockaddr *>(&clientinfo), &length) == -1)
				{
					std::cout << "getsockname: " << errno << std::endl;
				}
				std::cout << "port: " << ntohs(clientinfo.sin_port) << "\n" << "ip address: "  << clientinfo.sin_addr.s_addr << std::endl;
				char buffer;
				while (true)
				{
					ssize_t bytes_read = read(session, &buffer, 1);
					if (bytes_read > 0)
					{
						write(STDOUT_FILENO, &buffer, 1);
						continue;
					}
					if (bytes_read == 0)
					{
						close(session);
						removeClientfd(session, this->listSocks, this->clientToListen);
						printMapClientfds(this->listSocks, this->clientToListen);
						break;
					}
					if (errno == EAGAIN)
					{
						break;
					}
					removeClientfd(session, this->listSocks, this->clientToListen);
					close(session);
					break;
				}
				
			}
			std::cout << "i am here about to end the for loop iteration" << std::endl;
		}
	}
}

Multiplexer::~Multiplexer() {
	std::cout << "Multiplexer object destroyed" << std::endl;
}