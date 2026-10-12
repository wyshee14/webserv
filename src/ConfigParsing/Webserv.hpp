#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <iostream>
# include "Token.hpp"
# include "ServerConfig.hpp" 
# include "LocationConfig.hpp" 

class Webserv {
	private:
		std::vector<ServerConfig> _servers;
	public:
		Webserv(const std::string &configPath);
		
		~Webserv();
		
		//method to set up listening socket and bind it to port specified in ServerConfig.
		const std::vector<ServerConfig> &getServers() const;
};

#endif
