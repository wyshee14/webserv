#include "RequestConfig.hpp"
#include "ServerConfig.hpp"

RequestConfig::RequestConfig(const std::string &requestPath, const ServerConfig &serverConfig)
	: _path(requestPath), _serverConfig(&serverConfig), _autoindex(false),
	  _clientMaxBodySize(0), _redirectTarget(), _redirectCode(0), _hasLocation(false)
{
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	setConfig();
}

RequestConfig::RequestConfig(const RequestConfig &copy)
	: _path(copy._path), _serverConfig(copy._serverConfig),
	  _allowedMethods(copy._allowedMethods),
	  _redirectTarget(copy._redirectTarget), _redirectCode(copy._redirectCode),
	  _hasLocation(copy._hasLocation)
{
	_root = copy._root;
	_indexFiles = copy._indexFiles;
	_errorPage = copy._errorPage;
	_autoindex = copy._autoindex;
	_clientMaxBodySize = copy._clientMaxBodySize;
}

RequestConfig &RequestConfig::operator=(const RequestConfig &other)
{
	if (this != &other)
	{
		_path = other._path;
		_serverConfig = other._serverConfig;
		_root = other._root;
		_indexFiles = other._indexFiles;
		_errorPage = other._errorPage;
		_autoindex = other._autoindex;
		_clientMaxBodySize = other._clientMaxBodySize;
		_allowedMethods = other._allowedMethods;
		_redirectTarget = other._redirectTarget;
		_redirectCode = other._redirectCode;
		_hasLocation = other._hasLocation;
	}
	return (*this);
}

RequestConfig::~RequestConfig() {}

// Starts from the server block, then lets a matching location block override
// only the directives it actually defined.
void RequestConfig::setConfig()
{
	// set all the config as server first
	_root = _serverConfig->getRoot();
	_indexFiles = _serverConfig->getIndex();
	_errorPage = _serverConfig->getErrorPage();
	_autoindex = _serverConfig->getAutoindex();
	_clientMaxBodySize = _serverConfig->getClientMaxBodySize();

	// find the location path that matched the path that clients sends
	const LocationConfig *location = _serverConfig->findLocation(_path);
	if (location == NULL)
		return ;

	_hasLocation = true;
	if (location->hasRoot())
		_root = location->getRoot();
	if (!location->getIndex().empty())
		_indexFiles = location->getIndex();
	const std::map<int, std::string> &locationErrors = location->getErrorPage();
	for (std::map<int, std::string>::const_iterator it = locationErrors.begin();
		 it != locationErrors.end(); ++it)
		_errorPage[it->first] = it->second;
	if (location->hasAutoindex())
		_autoindex = location->getAutoindex();
	if (location->hasClientMaxBodySize())
		_clientMaxBodySize = location->getClientMaxBodySize();
	if (location->hasAllowedMethods())
		_allowedMethods = location->getAllowedMethods();
	if (location->hasRedirect())
	{
		_redirectCode = location->getRedirectCode();
		_redirectTarget = location->getRedirectTarget();
	}
}

const std::string &RequestConfig::getPath() const
{
	return _path;
}

const std::map<int, std::string> &RequestConfig::getErrorPage() const
{
	return _errorPage;
}

const size_t &RequestConfig::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

const bool &RequestConfig::getAutoindex() const
{
	return _autoindex;
}

const std::vector<std::string> &RequestConfig::getIndex() const
{
	return _indexFiles;
}

const std::string &RequestConfig::getRoot() const
{
	return _root;
}

const int &RequestConfig::getRedirectCode() const
{
	return _redirectCode;
}

const std::string &RequestConfig::getRedirectTarget() const
{
	return _redirectTarget;
}

const std::vector<std::string> &RequestConfig::getAllowedMethods() const
{
	return _allowedMethods;
}

bool RequestConfig::hasLocation() const
{
	return _hasLocation;
}

// <-----TO BE REMOVED (FOR DEBUGGING PURPOSE ONLY)----->
void printRequestConfig(const RequestConfig &config)
{
	std::cout << "request ";
	if (config.getPath().empty())
		std::cout << "null";
	else
		std::cout << config.getPath();
	std::cout << " {" << std::endl;

	std::cout << "    root: ";
	if (config.getRoot().empty())
		std::cout << "null";
	else
		std::cout << config.getRoot();
	std::cout << std::endl;

	std::cout << "    index: ";
	const std::vector<std::string> &indexFiles = config.getIndex();
	if (indexFiles.empty())
		std::cout << "null";
	for (std::vector<std::string>::const_iterator it = indexFiles.begin();
		 it != indexFiles.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::cout << "    error_page:";
	const std::map<int, std::string> &errorPages = config.getErrorPage();
	if (errorPages.empty())
		std::cout << " null";
	else
	{
		for (std::map<int, std::string>::const_iterator it = errorPages.begin();
			 it != errorPages.end(); ++it)
		{
			std::cout << " " << it->first << " " << it->second;
		}
	}
	std::cout << std::endl;

	std::cout << "    autoindex: "
			  << (config.getAutoindex() ? "true" : "false")
			  << std::endl;

	std::cout << "    client_max_body_size: "
			  << config.getClientMaxBodySize()
			  << std::endl;

	std::cout << "    allowed_methods: ";
	const std::vector<std::string> &allowedMethods = config.getAllowedMethods();
	if (allowedMethods.empty())
		std::cout << "null";
	for (std::vector<std::string>::const_iterator it = allowedMethods.begin();
		 it != allowedMethods.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::cout << "    redirect_code: ";
	if (config.getRedirectTarget().empty())
		std::cout << "null";
	else
		std::cout << config.getRedirectCode();
	std::cout << std::endl;

	std::cout << "    redirect_target: ";
	if (config.getRedirectTarget().empty())
		std::cout << "null";
	else
		std::cout << config.getRedirectTarget();
	std::cout << std::endl;

	std::cout << "    has_location: "
			  << (config.hasLocation() ? "true" : "false")
			  << std::endl;

	std::cout << "}" << std::endl;
}
