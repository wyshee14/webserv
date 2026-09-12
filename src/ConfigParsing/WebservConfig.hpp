#ifndef WEBSERVCONFIG_HPP
# define WEBSERVCONFIG_HPP

# include <vector>
# include "ServerConfig.hpp" 

class WebservConfig {
    private:
        std::vector<ServerConfig> _servers;

    public:
        WebservConfig();
        WebservConfig(const WebservConfig &copy);
        WebservConfig &operator=(const WebservConfig &other);
        ~WebservConfig(); 
        
        const std::vector<ServerConfig> &getServers() const;
};

#endif
