# include "ServerConfig.hpp"

ServerConfig::ServerConfig() 
{

}

ServerConfig::ServerConfig(const ServerConfig &copy)
    : _port(copy._port), _root(copy._root), _indexFiles(copy._indexFiles),
      _errorPage(copy._errorPage), _autoindex(copy._autoindex),
      _clientMaxBodySize(copy._clientMaxBodySize),
      _locations(copy._locations), _directives(copy._directives)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        _port = other._port;
        _root = other._root;
        _indexFiles = other._indexFiles;
        _errorPage = other._errorPage;
        _autoindex = other._autoindex;
        _clientMaxBodySize = other._clientMaxBodySize;
        _locations = other._locations;
        _directives = other._directives;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

void ServerConfig::addDirectives(const Directive &directive)
{
    (void)directive;
}