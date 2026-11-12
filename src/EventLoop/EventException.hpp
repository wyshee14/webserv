#ifndef EVENTEXCEPTION_HPP
# define EVENTEXCEPTION_HPP

# include <iostream>
# include <exception>

class EventException : public std::exception {
    private:
        int _errorCode;
        int _saveErrno;
        int _addrError;
        std::string _errorMessage;
    public:
        typedef enum {
            SOCKETSETUPEXCEPTION,
            BINDEXCEPTION,
            GETADDRINFOEXCEPTION,
            SOCKOPTEXCEPTION,
            LISTENEXCEPTION,
            EPOLLCREATIONEXCEPTION,
            EPOLLWAITEXCEPTION,
            ACCEPTEXCEPTION,
            LISTENSOCKETEPOLLCTLADDEXCEPTION,
            CLIENTSOCKETEPOLLCTLADDEXCEPTION,
        };
        EventException();

        const char *what() const throw();

        const int& getErrorCode() const;
        const int getSaveErrno() const;
        void setSaveErrno(int errnocode);
        const int getAddrError() const;
        void setAddrError(int addrerr);

        ~EventException();
};

#endif