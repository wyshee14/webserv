# include "Webserv.hpp"
# include "ConfigParsing/ConfigFile.hpp"
# include "ConfigParsing/ConfigParser.hpp"

Webserv::Webserv(const std::string &configPath)
{
    ConfigFile config(configPath);
    config.processConfigFile();
    ConfigParser parser(config.getTokens());
    _servers = parser.parseTokens();
	validateAllServersListenEndpoints(_servers);
    // printAllConfiguration(_servers);
}

Webserv::~Webserv() {}

const std::vector<ServerConfig> &Webserv::getServers() const
{
    return _servers;
}
