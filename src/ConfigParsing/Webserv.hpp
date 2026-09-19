#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include "ConfigFile.hpp"
# include "ServerConfig.hpp" 

class Webserv {
    private:
        std::vector<ServerConfig> _servers;

    public:
        Webserv(const std::string &configPath);
        ~Webserv(); 
        
        const std::vector<ServerConfig> &getServers() const;
};

#endif
