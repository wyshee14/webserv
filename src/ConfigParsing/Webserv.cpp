# include "Webserv.hpp"
# include "ConfigFile.hpp"
# include "ConfigParser.hpp"

Webserv::Webserv(const std::string &configPath)
{
    ConfigFile config(configPath);
    config.processConfigFile();
    ConfigParser parser(config.getTokens());
    _servers = parser.parseTokens();
}

Webserv::~Webserv() {}

const std::vector<ServerConfig> &Webserv::getServers() const
{
    return _servers;
}
