# include "ServerConfig.hpp"
# include <stdlib.h>    // library for strtoul

ServerConfig::ServerConfig() 
{
    _autoindex = false;
    _clientMaxBodySize = 1000000;
}

ServerConfig::ServerConfig(const ServerConfig &copy)
    : _listenEndpoint(copy._listenEndpoint), _root(copy._root), _indexFiles(copy._indexFiles),
      _errorPage(copy._errorPage), _autoindex(copy._autoindex),
      _clientMaxBodySize(copy._clientMaxBodySize),
      _locations(copy._locations)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        _listenEndpoint = other._listenEndpoint;
        _root = other._root;
        _indexFiles = other._indexFiles;
        _errorPage = other._errorPage;
        _autoindex = other._autoindex;
        _clientMaxBodySize = other._clientMaxBodySize;
        _locations = other._locations;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

void ServerConfig::addDirectives(const Directive &directive)
{
    if (directive.key == "listen")
        setListenEndpoints(directive.values);
}

const std::vector<ListenDirective> &ServerConfig::getListenEndpoint() const
{
    return _listenEndpoint;
}

void ServerConfig::setListenEndpoints(const std::vector<std::string> &values)
{
    std::string address;
    std::string port;
    size_t separator = values[0].find(':');
    if (separator == std::string::npos)
    {
        address = "0.0.0.0";
        port = values[0];
    }
    else 
    {    
        address = values[0].substr(0, separator);
        port = values[0].substr(separator + 1);
    }
    uint16_t portNumber = strtoul(port.c_str(), NULL, 10);
    _listenEndpoint.push_back(ListenDirective(address, portNumber));
}

const std::vector<LocationConfig> &ServerConfig::getLocations() const
{
    return _locations;
}

void ServerConfig::addLocation(LocationConfig &location)
{
    _locations.push_back(location);
}