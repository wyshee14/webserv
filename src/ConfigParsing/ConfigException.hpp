#ifndef CONFIGEXCEPT_HPP
# define CONFIGEXCEPTION_HPP

# include <exception>
# include <iostream>

#define RESET "\033[0m"
#define RED "\033[31m"

class ConfigException : public std::exception {    
    public:
        enum errorType {
            INVALID_FILE,
            UNEXPECTED_TOKEN,
            UNKNOWN_DIRECTIVE,
            WRONG_DIRECTIVE_CONTEXT,
            INVLAID_DIRECTIVE_VALUES,
        };
        ConfigException(errorType err);
        ~ConfigException() throw();
        const char* what() const throw();

    private:
        errorType _error;
    
};

#endif
