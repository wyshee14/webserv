#include "EventException.hpp"

EventException::EventException(int errcode, ExceptionList enumtype) : _saveErrno(errcode), _errType(enumtype) {
    switch (_errType)
    {
        case SOCKETSETUPEXCEPTION:
            _errorMessage = "Socket setup failure: ";
            break;
        case BINDEXCEPTION:
            _errorMessage = "Bind failure: ";
            break;
        case GETADDRINFOEXCEPTION:
            _errorMessage = "Getaddrinfo failure: ";
            break;
        case SOCKOPTEXCEPTION:
            _errorMessage = "SockOpt failure: ";
            break;
        case LISTENEXCEPTION:
            _errorMessage = "Listen failure: ";
            break;
        case EPOLLCREATIONEXCEPTION:
            _errorMessage = "Epoll_create failure: ";
            break;
        case EPOLLWAITEXCEPTION:
            _errorMessage = "Epoll_wait failure: ";
            break;
        case ACCEPTEXCEPTION:
            _errorMessage = "Accept failure: ";
            break;
        case LISTENSOCKETEPOLLCTLADDEXCEPTION:
            _errorMessage = "ListenSocket Epoll_Ctl_Add failure: ";
            break;
        case CLIENTSOCKETEPOLLCTLADDEXCEPTION:
            _errorMessage = "Client Socket Epoll_Ctl_Add failure: ";
            break;
    }
}

const char * EventException::what() const throw() {
    return (_errorMessage.c_str());
}

int EventException::getSaveErrno() const {
    return (_saveErrno);
}

EventException::ExceptionList EventException::getExceptionType() const {
    return (_errType);
}

void EventException::setSaveErrno(int errnocode) {
    this->_saveErrno = errnocode;
}

EventException::~EventException() throw () {}