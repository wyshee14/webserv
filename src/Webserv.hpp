#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include "ConfigParsing/ServerConfig.hpp"
# include "ConfigParsing/LocationConfig.hpp"
# include "ConfigParsing/RequestConfig.hpp"

class Webserv {
    private:
        std::vector<ServerConfig> _servers;

    public:
        Webserv(const std::string &configPath);
        ~Webserv();

        const std::vector<ServerConfig> &getServers() const;
		void processConfig(const std::string &configPath);
		RequestConfig requestConfig(const std::string &requestPath, size_t serverIndex = 0) const;
		size_t findServerByEndpoint(const std::string &localIp, uint16_t localPort) const;

};

#endif
