# include <stdexcept>
# include "Webserv.hpp"
# include "ConfigParsing/ConfigFile.hpp"
# include "ConfigParsing/ConfigParser.hpp"

Webserv::Webserv(const std::string &configPath)
{
	processConfig(configPath);
	//hardcoded, to be parse to http part, this part to be removed
	const std::string address = "0.0.0.0";
	uint16_t port = 8080;
	const std::string path = "/images";
	// only this line is requirred, http should check the _hasLocation, if false return 404, if true use the config
	RequestConfig request = requestConfig(path, findServerByEndpoint(address, port));
	// remove this line
	printRequestConfig(request);
}

Webserv::~Webserv() {}

const std::vector<ServerConfig> &Webserv::getServers() const
{
	return _servers;
}

void Webserv::processConfig(const std::string &configPath)
{
	ConfigFile config(configPath);
	config.processConfigFile();
	ConfigParser parser(config.getTokens());
	_servers = parser.parseTokens();
	validateAllServersListenEndpoints(_servers);
	// printAllConfiguration(_servers);
}

RequestConfig Webserv::requestConfig(const std::string &requestPath, size_t serverIndex) const
{
	return RequestConfig(requestPath, _servers.at(serverIndex));
}

// Search for the listen address and port that the client sends request
size_t Webserv::findServerByEndpoint(const std::string &raddress, uint16_t rport) const
{
	for (size_t i = 0; i < _servers.size(); ++i)
	{
		const std::vector<ListenDirective> &listen = _servers[i].getListenEndpoint();
		for (size_t j = 0; j < listen.size(); ++j)
		{
			if (listen[j].port == rport && listen[j].address == raddress)
				return i;
		}
	}
	throw std::out_of_range("No server configuration matches the endpoint");
}
