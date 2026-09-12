#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <vector>

class LocationConfig {
    private:
        std::string _path;
        std::string _root;
        std::string _indexFiles;
        std::vector<std::string> _allowedMethods;
        bool _autoindex;
        std::string _redirectTarget;
        int _redirectCode;
        size_t _clientMaxBodySize;

    public:
        LocationConfig();
        LocationConfig(const LocationConfig &copy);
        LocationConfig &operator=(const LocationConfig &other);
        ~LocationConfig();

        const std::string &getPath() const;
        const std::string &getRoot() const;
        bool isAutoindexEnabled() const;
};

#endif
