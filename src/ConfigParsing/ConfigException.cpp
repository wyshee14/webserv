# include "ConfigException.hpp"

ConfigException::ConfigException(errorType err) : _error(err) {}

ConfigException::~ConfigException() throw() {}

const char* ConfigException::what() const throw()
{
    switch (_error){
        case INVALID_FILE:
            return "Invalid configuration file";
        case UNEXPECTED_TOKEN: 
            return "Unexpected token in config file";
        case UNKNOWN_DIRECTIVE:
            return "Unknown Directive";
        case WRONG_DIRECTIVE_CONTEXT:
            return "Directive is is the wrong context";
        case INVLAID_DIRECTIVE_VALUES:
            return "Directive values are invalid";
    }
    return "Unknown configuration error";
}