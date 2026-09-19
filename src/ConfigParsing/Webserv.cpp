# include "Webserv.hpp"

Webserv::Webserv(const std::string &configPath)
{
    ConfigFile config(configPath);
    config.processConfigFile();
}

Webserv::~Webserv() {}
