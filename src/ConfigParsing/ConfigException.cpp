# include "ConfigException.hpp"

ConfigException::ConfigException(errorType err, const std::string &message) : _error(err) 
{
    switch (_error) {
        case INVALID_FILE:
            _message = "Invalid configuration file: ";
            break;
        case INVALID_DIRECTORY:
            _message = "Invalid directory: ";
            break;        
        case UNEXPECTED_TOKEN: 
            _message = "Unexpected token in config file: ";
            break;
        case UNKNOWN_DIRECTIVE:
            _message = "Unknown Directive: ";
            break;
        case WRONG_DIRECTIVE_CONTEXT:
            _message = "Directive is is the wrong context: ";
            break;
        case INVALID_DIRECTIVE_VALUES:
            _message = "Directive values are invalid: ";
            break;
        case INVALID_PORT_NUMBER:
            _message = "Invalid port number: ";
            break;
        case INVALID_IP_ADDRESS:
            _message = "Invalid IP address: ";
            break;
        case DUPLICATE_DIRECTIVE:
            _message = "Duplicate directive found: ";
            break;
        case INVALID_STATUS_CODE:
            _message = "Invalid status code: ";
            break;
        case MISCONFIGURED_ERROR_FILE:
            _message = "Misconfigured error file. ";
            break;
        case INVALID_REDIRECT_PATH:
            _message = "Invalid redirect path: ";
            break;
        default:
            _message = "Unknown configuration error: ";
            break;
    }
    _message += message;
}

ConfigException::~ConfigException() throw() {}

const char* ConfigException::what() const throw()
{
    return _message.c_str();
}