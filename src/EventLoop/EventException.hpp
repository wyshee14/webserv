#ifndef EVENTEXCEPTION_HPP
# define EVENTEXCEPTION_HPP

# include <iostream>
# include <exception>

class EventException : public std::exception {
    public:
        enum ExceptionList {
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
        EventException(int errcode, ExceptionList enumtype);
        
        const char *what() const throw();
        
        int getSaveErrno() const;
        void setSaveErrno(int errnocode);
        ExceptionList getExceptionType() const;
        
        ~EventException() throw();
    private:
            int _saveErrno;
            ExceptionList _errType;
            std::string _errorMessage;
};

#endif