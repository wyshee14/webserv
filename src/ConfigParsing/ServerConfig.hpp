#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

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
        // std::string _serverName;
        std::string _root;
        std::vector<std::string> _indexFiles;
        std::map<int, std::string> _errorPage;
        bool _autoindex;
        size_t _clientMaxBodySize;
        std::vector<LocationConfig> _locations;

    public:
        ServerConfig();
        ServerConfig(std::vector<Directive> _directives); 
        ServerConfig(const ServerConfig &copy);
        ServerConfig &operator=(const ServerConfig &other);
        ~ServerConfig();

        // Getter
        const std::vector<ListenDirective> &getListenEndpoint() const;
        const std::string &getRoot() const;
        const std::vector<LocationConfig> &getLocations() const;
        const LocationConfig* findLocation(const std::string &uri) const;

        // Setter
        void setListenEndpoints(const std::vector<std::string> &values);
        void setRoot();

        void addLocation(LocationConfig &location);
        void addDirectives(const Directive &directive);

};

#endif
