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
      _locations(copy._locations),
      _hasRoot(copy._hasRoot), _hasAutoindex(copy._hasAutoindex),
      _hasClientMaxBodySize(copy._hasClientMaxBodySize)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        _listenEndpoint = other.getListenEndpoint();
        _root = other.getRoot();
        _indexFiles = other.getIndex();
        _errorPage = other.getErrorPage();
        _autoindex = other.getAutoindex();
        _clientMaxBodySize = other.getClientMaxBodySize();
        _locations = other.getLocations();
        _hasRoot = other._hasRoot;
        _hasAutoindex = other._hasAutoindex;
        _hasClientMaxBodySize = other._hasClientMaxBodySize;
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
        int status = atoi(statusCode[i].c_str());
        _errorPage.insert(std::make_pair(status, file));
    }
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
}

const bool &ServerConfig::getAutoindex() const
{
    return _autoindex;
}

void ServerConfig::setAutoindex(const std::string &value)
{
    if (value == "on")
        _autoindex = true;
}

const std::vector<std::string> &ServerConfig::getIndex() const
{
    return _indexFiles;
}

void ServerConfig::setIndex(const std::vector<std::string> &index)
{
    // combine the values if got duplicate index entry
    _indexFiles.insert(_indexFiles.end(), index.begin(), index.end());
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
    uint16_t portNumber = strtoul(port.c_str(), NULL, 10);
    // Check listen endpoint are duplicates
    for (std::vector<ListenDirective>::const_iterator it = _listenEndpoint.begin(); it != _listenEndpoint.end(); it++)
    {
        if (it->address == address && it->port == portNumber)
            throw ConfigException(ConfigException::DUPLICATE_DIRECTIVE, value);
    }
    _listenEndpoint.push_back(ListenDirective(address, portNumber));
}

const std::vector<LocationConfig> &ServerConfig::getLocations() const
{
    return _locations;
}

void ServerConfig::addLocation(LocationConfig &location)
{
    for (std::vector<LocationConfig>::const_iterator it = _locations.begin();
         it != _locations.end(); ++it)
    {
        if (it->getPath() == location.getPath())
            throw ConfigException(ConfigException::DUPLICATE_DIRECTIVE,
                                  "location " + location.getPath());
    }
    _locations.push_back(location);
}

const LocationConfig *ServerConfig::findLocation(const std::string &uri) const
{
    const LocationConfig *best = NULL;
    size_t bestLength = 0;

    for (std::vector<LocationConfig>::const_iterator it = _locations.begin();
         it != _locations.end(); ++it)
    {
        const std::string &path = it->getPath();
        // longest matching prefix wins, e.g. /images beats /
        if (uri.compare(0, path.size(), path) == 0 && path.size() > bestLength)
        {
            best = &(*it);
            bestLength = path.size();
        }
    }
    return best;
}
