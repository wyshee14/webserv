#include "MasterProcess.hpp"
#include <exception>

MasterProcess::MasterProcess() {
	std::cout << "MasterProcess Object created" << std::endl;
}

MasterProcess::MasterProcess(const std::vector<ServerConfig>& obj) {
	this->config = obj;
}

void MasterProcess::errorCloseSockets() {
	std::vector<ListenSocket>::iterator it;
	for (it = lsocs.begin(); it < lsocs.end(); it++)
	{
		if (it->GetPass() == false && it->Getlsfd() != -1)
			close(it->Getlsfd());
	}
}

int MasterProcess::SocketBuilder() {
	std::cout << "size of config " << config.size() << std::endl;
	std::cout << "entering SocketBuilder" << std::endl;
	size_t totalSockets = 0;
	for (size_t i = 0; i < config.size(); i++)
	{
		totalSockets += config[i].getListenEndpoint().size();
	}
	lsocs.reserve(totalSockets);
	for (size_t i = 0; i < config.size(); i++)
	{
		const std::vector<ListenDirective> &ref = config[i].getListenEndpoint();
		for (size_t i = 0; i < ref.size(); i++)
		{
			std::cout << lsocs.size() << std::endl;
			std::cout << ref[i].port << std::endl;
			std::cout << ref[i].address << std::endl;
			lsocs.push_back(ListenSocket(ref[i].port, ref[i].address));
		}
		std::cout << "-----------------one iteration of directive done--------------\n";
	}
	try {
		for (size_t i = 0; i < lsocs.size(); i++)
		{
			lsocs[i].socketSetup();
			lsocs[i].SetPass(true);
		}
	}
	catch (EventException& e) {
		errorCloseSockets();
		if (e.getExceptionType() == EventException::GETADDRINFOEXCEPTION)
		{
			std::cerr << e.what() << gai_strerror(e.getSaveErrno()) << std::endl;
		}
		std::cerr << e.what() << strerror(e.getSaveErrno()) << std::endl;
		
		return (1);
	}
	catch (std::exception& e) {
		errorCloseSockets();
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

std::vector<ListenSocket>& MasterProcess::getlsocs() {
	return (this->lsocs);
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterProcess Object deconstructed" << std::endl;
}

// int main(int argc, char **argv)
// {
// 	if (argc == 2)
// 	{
// 		size_t quant = atoi(argv[1]);
// 		MasterProcess master;

// 		master.SocketBuilder();

// 		std::cout << master.getlsocs().size() << "  i am moving on from socketbuilder" << std::endl;
// 		std::vector<ListenSocket> &lsock = master.getlsocs();
// 		for (size_t i = 0; i < lsock.size(); i++)
// 		{
// 			std::cout << lsock[i].GetPortNum() << std::endl;
// 		}
// 	}
// }