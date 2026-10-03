# include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}

LocationConfig::LocationConfig(const LocationConfig &copy)
	: _path(copy._path), _root(copy._root), _indexFiles(copy._indexFiles),
	  _allowedMethods(copy._allowedMethods), _autoindex(copy._autoindex),
	  _redirectTarget(copy._redirectTarget), _redirectCode(copy._redirectCode),
	  _clientMaxBodySize(copy._clientMaxBodySize)
{
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		_path = other._path;
		_root = other._root;
		_indexFiles = other._indexFiles;
		_allowedMethods = other._allowedMethods;
		_autoindex = other._autoindex;
		_redirectTarget = other._redirectTarget;
		_redirectCode = other._redirectCode;
		_clientMaxBodySize = other._clientMaxBodySize;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}