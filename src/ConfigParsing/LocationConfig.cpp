# include "LocationConfig.hpp"
# include "ConfigException.hpp"
# include <cstdlib> // library for atoi

LocationConfig::LocationConfig() 
{
	_autoindex = false;
    _clientMaxBodySize = 1000000;       // Default 1 megabyte
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
    _hasRoot = false;
    _hasAutoindex = false;
    _hasClientMaxBodySize = false;
	_hasAllowedMethods = false;
	_hasRedirect = false;
}

LocationConfig::LocationConfig(const LocationConfig &copy)
	: _path(copy._path),_root(copy._root), _indexFiles(copy._indexFiles),
      _errorPage(copy._errorPage), _autoindex(copy._autoindex),
      _clientMaxBodySize(copy._clientMaxBodySize),
	  _allowedMethods(copy._allowedMethods),
	  _redirectTarget(copy._redirectTarget), _redirectCode(copy._redirectCode)
{
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		_path = other._path;
		_root = other._root;
		_indexFiles = other._indexFiles;
		_errorPage = other._errorPage;
		_autoindex = other._autoindex;
		_clientMaxBodySize = other._clientMaxBodySize;
		_allowedMethods = other._allowedMethods;
		_redirectTarget = other._redirectTarget;
		_redirectCode = other._redirectCode;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

const int &LocationConfig::getRedirectCode() const
{
	return _redirectCode;
}

const std::string &LocationConfig::getRedirectTarget() const
{
	return _redirectTarget;
}

void LocationConfig::setRedirection(const std::vector<std::string> &values)
{
	_redirectCode = atoi(values[0].c_str());
	_redirectTarget = values[1];
}

const std::vector<std::string> &LocationConfig::getAllowedMethods() const
{
	return _allowedMethods;
}

void LocationConfig::setAllowedMethods(const std::vector<std::string> &values)
{
	_allowedMethods = values;
}

const std::map<int, std::string> &LocationConfig::getErrorPage() const
{
    return _errorPage;
}

void LocationConfig::setErrorPage(const std::vector<std::string> &values)
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

const size_t &LocationConfig::getClientMaxBodySize() const
{
    return _clientMaxBodySize;
}

void LocationConfig::setClientMaxBodySize(std::string value)
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

const bool &LocationConfig::getAutoindex() const
{
    return _autoindex;
}

void LocationConfig::setAutoindex(const std::string &value)
{
    if (value == "on")
        _autoindex = true;
}

const std::vector<std::string> &LocationConfig::getIndex() const
{
    return _indexFiles;
}

void LocationConfig::setIndex(const std::vector<std::string> &index)
{
    // combine the values if got duplicate index entry
    _indexFiles.insert(_indexFiles.end(), index.begin(), index.end());
}

const std::string &LocationConfig::getRoot() const
{
    return _root;
}

void LocationConfig::setRoot(const std::string &value)
{
    _root = value;
}


void LocationConfig::addDirectives(const Directive &directive)
{
    if (directive.key == "root" && !_hasRoot)
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
	else if (directive.key == "return" && !_hasRedirect)
	{
		setRedirection(directive.values);
		_hasRedirect = true;
	}
	else if (directive.key == "allowed_methods" && !_hasAllowedMethods)
	{
		setAllowedMethods(directive.values);
		_hasAllowedMethods = true;
	}
    else
        throw ConfigException(ConfigException::DUPLICATE_DIRECTIVE, directive.key);
}

const std::string &LocationConfig::getPath() const
{
	return _path;
}

void LocationConfig::setPath(const std::string &path)
{
	if (path.empty())
		throw (ConfigException(ConfigException::INVALID_LOCATION_PATH));

	if (path[0] != '/')
		throw (ConfigException(ConfigException::INVALID_LOCATION_PATH, path));

	if (path.find_first_of(";,") != std::string::npos)
		throw (ConfigException(ConfigException::INVALID_LOCATION_PATH, path));

	_path = path;
}