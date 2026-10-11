# include "ServerConfig.hpp"
# include <stdlib.h>    // library for strtoul
# include "ConfigException.hpp"

ServerConfig::ServerConfig() 
{
    _autoindex = false;
    _clientMaxBodySize = 1000000;
    _hasRoot = false;
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
        setListenEndpoints(directive.values[0]);
    else if (directive.key == "root" && !_hasRoot)
    {
        setRoot(directive.values[0]);
        _hasRoot = true;
    }
    else
        throw ConfigException(ConfigException::DUPLICATE_DIRECTIVE, directive.key);
}

void ServerConfig::setRoot(const std::string &value)
{
    _root = value;
}

const std::vector<ListenDirective> &ServerConfig::getListenEndpoint() const
{
    return _listenEndpoint;
}

void ServerConfig::setListenEndpoints(const std::string &value)
{
    std::string address;
    std::string port;
    size_t separator = value.find(':');
    if (separator == std::string::npos)
    {
        address = "0.0.0.0";
        port = value;
    }
    else 
    {    
        address = value.substr(0, separator);
        port = value.substr(separator + 1);
    }
    std::cout << "address: " << address << ", port: " << port <<std::endl;
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