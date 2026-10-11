# include "ServerConfig.hpp"
# include <stdlib.h>    // library for strtoul
# include "ConfigException.hpp"

ServerConfig::ServerConfig() 
{
    _autoindex = false;
    _clientMaxBodySize = 1000000;       // Default 1 megabyte
    _hasRoot = false;
    _hasAutoindex = false;
    _hasClientMaxBodySize = false;
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
    else if (directive.key == "index")
    {
        setIndex(directive.values);
    }
    else if (directive.key == "autoindex" && !_hasAutoindex)
    {
        setAutoindex(directive.values[0]);
        _hasAutoindex = true;
    }
    else if (directive.key == "client_max_body_size" && !_hasClientMaxBodySize)
    {
        setClientMaxBodySize(directive.values[0]);
        _hasClientMaxBodySize = true;
    }
    else if (directive.key == "error_page")
    {
        setErrorPage(directive.values);
    }  
    else
        throw ConfigException(ConfigException::DUPLICATE_DIRECTIVE, directive.key);
}

const std::map<int, std::string> &ServerConfig::getErrorPage() const
{
    return _errorPage;
}

void ServerConfig::setErrorPage(const std::vector<std::string> &values)
{
    std::vector<std::string> statusCode = values;
    statusCode.pop_back();
    std::string file = values[values.size() - 1];
    for(size_t i = 0; i < statusCode.size(); i++)
    {
        size_t status = strtoul(statusCode[i].c_str(), NULL, 10);
        _errorPage.insert(std::make_pair(status, file));
    }
    // print map
    // for (std::map<int, std::string>::const_iterator it = _errorPage.begin(); it != _errorPage.end(); ++it)
    // {
    //     std::cout << "status: " << it->first << ", file: " << it->second << std::endl;
    // }
}

const size_t &ServerConfig::getClientMaxBodySize() const
{
    return _clientMaxBodySize;
}

void ServerConfig::setClientMaxBodySize(std::string value)
{
    char suffix = value[value.size() - 1];
    if (!std::isdigit(suffix))
        value.erase(value.size() - 1);
    size_t size = strtoul(value.c_str(), NULL, 10);
    if (suffix == 'K')
        size *= 1000;
    else if (suffix == 'M')
        size *= 1000000;
    else if (suffix == 'G')
        size *= 1000000000;
    _clientMaxBodySize = size;
    std::cout << "client max body size: " << getClientMaxBodySize() << std::endl;
}

const bool &ServerConfig::getAutoindex() const
{
    return _autoindex;
}

void ServerConfig::setAutoindex(const std::string &value)
{
    if (value == "true")
        _autoindex = true;
    else
        _autoindex = false;
}

const std::vector<std::string> &ServerConfig::getIndex() const
{
    return _indexFiles;
}

void ServerConfig::setIndex(const std::vector<std::string> &index)
{
    // combine the values if got duplicate index entry
    _indexFiles.insert(_indexFiles.end(), index.begin(), index.end());
    // std::cout << "index: ";
    // for (std::vector<std::string>::iterator it = _indexFiles.begin(); it != _indexFiles.end(); it++)
    // {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
}

const std::string &ServerConfig::getRoot() const
{
    return _root;
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