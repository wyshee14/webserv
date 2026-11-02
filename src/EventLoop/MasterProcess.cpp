#include "MasterProcess.hpp"

MasterProcess::MasterProcess() {
	std::cout << "MasterProcess Object created" << std::endl;
}

MasterProcess::MasterProcess(const std::vector<ServerConfig>& obj) {
	this->config = obj;
}

void MasterProcess::SocketBuilder() {
	std::cout << "size of config " << config.size() << std::endl;
	std::cout << "entering SocketBuilder" << std::endl;
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
	for (size_t i = 0; i < lsocs.size(); i++)
	{
		lsocs[i].socketSetup();
	}
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