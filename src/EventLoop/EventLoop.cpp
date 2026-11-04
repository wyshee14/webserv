#include "EventLoop.hpp"

EventLoop::EventLoop() {
	std::cout << "EventLoop object created" << std::endl;
}

void EventLoop::coreMechanic() {
	;
}

EventLoop::~EventLoop() {
	std::cout << "EventLoop object destroyed" << std::endl;
}
