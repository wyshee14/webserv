#include "MasterProcess.hpp"

MasterProcess::MasterProcess() : port(8000) {
	std::cout << "MasterProcess object constructed" << std::endl;
}

void MasterProcess::SocketBuilder(size_t quant) {
	for (size_t i = 0; i < quant; i++)
	{
		lsocs.push_back(ListenSocket(port));
	}
	for (size_t i = 0; i < lsocs.size(); i++)
	{
		lsocs[i].socketSetup();
	}
}

std::vector<ListenSocket> & MasterProcess::getlsocs() {
	return (this->lsocs);
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterPRocess object deconstructed" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		size_t quant = atoi(argv[1]);
		MasterProcess master;

		master.SocketBuilder(quant);
		std::vector<ListenSocket> &lsock = master.getlsocs();
		for (size_t i = 0; i < lsock.size(); i++)
		{
			std::cout << lsock[i].GetPortNum() << std::endl;
		}
	}
}