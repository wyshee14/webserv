#include "Client.hpp"

Client::Client() : announce(IDLE), requestCount(0) {
	std::cout << "Client object created" << std::endl;
}

void Client::processBytes() {
	parserstate statement = parser.parsing(recvBuffer);
	std::cout << statement << std::endl;
	if (statement == RESPONSE)
	{
		std::cout << "----------i am inside process bytes and it is in response state-----------" << std::endl;
		announce = WRITING;
		parser.setParserState(READING_HEADER);
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
