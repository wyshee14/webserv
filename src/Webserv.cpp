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

int Webserv::Masterprocessor() {
	MasterProcess factory(getServers());
	if (factory.SocketBuilder() == 1)
		return (1);
	Multiplexer event(factory.getlsocs());
	event.eventLoop();
	return (0);
}

Webserv::~Webserv() {}

const std::vector<ServerConfig> &Webserv::getServers() const
{
	return _servers;
}
