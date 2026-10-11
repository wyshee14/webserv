#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "LocationConfig.hpp"
# include "Token.hpp"
# include "Directive.hpp"

// class LocationConfig;

class ServerConfig {
    private:
        std::vector<ListenDirective> _listenEndpoint;
        std::string _root;
        std::vector<std::string> _indexFiles;
        std::map<int, std::string> _errorPage;
        bool _autoindex;
        size_t _clientMaxBodySize;
        std::vector<LocationConfig> _locations;
        bool _hasRoot;
        bool _hasAutoindex;
        bool _hasClientMaxBodySize;

    public:
        ServerConfig();
        ServerConfig(const ServerConfig &copy);
        ServerConfig &operator=(const ServerConfig &other);
        ~ServerConfig();

        // Getter
        const std::vector<ListenDirective> &getListenEndpoint() const;
        const std::string &getRoot() const;
        const std::vector<std::string> &getIndex() const;
        const bool &getAutoindex() const;
        const size_t &getClientMaxBodySize() const;
        const std::vector<LocationConfig> &getLocations() const;
        const LocationConfig* findLocation(const std::string &uri) const;

        // Setter
        void setListenEndpoints(const std::string &value);
        void setRoot(const std::string &value);
        void setIndex(std::vector<std::string> index);
        void setAutoindex(const std::string &value);
        void setClientMaxBodySize(std::string value);

        void addLocation(LocationConfig &location);
        void addDirectives(const Directive &directive);

};

#endif
