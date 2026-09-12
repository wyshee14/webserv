#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <vector>
# include <stdint.h>    // Library for uint16_t
# include <map>
# include "LocationConfig.hpp"

// class LocationConfig;

class ServerConfig {
    private:
        uint16_t _port;     // our port only accept one number from 0-65535
        // std::string _serverName;
        std::string _root;
        std::vector<std::string> _indexFiles;
        std::map<int, std::string> _errorPage;
        bool _autoindex;
        size_t _clientMaxBodySize;
        std::vector<LocationConfig> _locations;

    public:
        ServerConfig();
        ServerConfig(const ServerConfig &copy);
        ServerConfig &operator=(const ServerConfig &other);
        ~ServerConfig();

        const std::string &getRoot() const;
        const std::vector<LocationConfig> &getLocations() const;
        const LocationConfig* findLocation(const std::string &uri) const;

};

#endif
