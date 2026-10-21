#ifndef CONFIGEXCEPTION_HPP
# define CONFIGEXCEPTION_HPP

# include <exception>
# include <iostream>

#define RESET "\033[0m"
#define RED "\033[31m"

class ConfigException : public std::exception {
    public:
        enum errorType {
            INVALID_FILE,
            INVALID_DIRECTORY,
            UNEXPECTED_TOKEN,
            UNKNOWN_DIRECTIVE,
            WRONG_DIRECTIVE_CONTEXT,
            INVALID_DIRECTIVE_VALUES,
            INVALID_PORT_NUMBER,
            INVALID_IP_ADDRESS,
            DUPLICATE_DIRECTIVE,
            INVALID_STATUS_CODE,
            MISCONFIGURED_ERROR_FILE,
            INVALID_REDIRECT_PATH,
            INVALID_LOCATION_PATH,
        };
        // dynamic message
        ConfigException(errorType err, const std::string &message = "");
        ~ConfigException() throw();
        const char* what() const throw();

    private:
        errorType _error;
        std::string _message;

};

#endif
