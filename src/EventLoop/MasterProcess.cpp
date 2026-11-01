#include "MasterProcess.hpp"

MasterProcess::MasterProcess() : port(8000) {
	std::cout << "MasterProcess Object created" << std::endl;
}

void MasterProcess::SocketBuilder(size_t quant) {
	// std::cout << lsocs.capacity() << std::endl;
	// std::cout << lsocs.capacity() << std::endl;
	for (size_t i = 0; i < quant; i++)
	{
		std::cout << lsocs.size() << std::endl;
		lsocs.push_back(ListenSocket(port));
		std::cout << "buffering..." << std::endl;
	}
	std::cout << "paused in the middle" << std::endl;
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

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		size_t quant = atoi(argv[1]);
		MasterProcess master;

		master.SocketBuilder(quant);

		std::cout << master.getlsocs().size() << "  i am moving on from socketbuilder" << std::endl;
		std::vector<ListenSocket> &lsock = master.getlsocs();
		for (size_t i = 0; i < lsock.size(); i++)
		{
			std::cout << lsock[i].GetPortNum() << std::endl;
		}
	}
}