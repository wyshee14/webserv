#include "Client.hpp"

Client::Client() : announce(IDLE), requestCount(0) {
	std::cout << "Client object created" << std::endl;
}

void Client::processBytes() {

	if (parser.parsing(recvBuffer) == RESPONSE)
	{
		announce = WRITING;
		parser.engine(sendBuffer);
	}
}

void Client::incrementCount(int count) {
	requestCount+= count;
}

void Client::setState(state annc) {
	this->announce = annc;
}

const state& Client::getState() {
	return (announce);
}

std::string& Client::getRecvBuffer() {
	return (recvBuffer);
}

std::string& Client::getSendBuffer() {
	return (sendBuffer);
}

Client::~Client() {
	std::cout << "Client object destroyed" << std::endl;
}
