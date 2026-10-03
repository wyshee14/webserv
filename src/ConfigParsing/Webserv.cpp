# include "Webserv.hpp"
# include "ConfigFile.hpp"
# include "ConfigParser.hpp"

Webserv::Webserv(const std::string &configPath)
{
    ConfigFile config(configPath);
    config.processConfigFile();
    ConfigParser parser;
    parser.parseTokens(config.getTokens());
}

Webserv::~Webserv() {}
