#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include "Directive.hpp"

class LocationConfig {
    private:
        std::string _path;
        std::string _root;
        std::vector<std::string> _indexFiles;
        std::map<int, std::string> _errorPage;
        bool _autoindex;
        size_t _clientMaxBodySize;
        std::vector<std::string> _allowedMethods;
        std::string _redirectTarget;
        int _redirectCode;
        bool _hasRoot;
        bool _hasAutoindex;
        bool _hasClientMaxBodySize;
        bool _hasAllowedMethods;
        bool _hasRedirect;

    public:
        LocationConfig();
        LocationConfig(const LocationConfig &copy);
        LocationConfig &operator=(const LocationConfig &other);
        ~LocationConfig();

        // Getter
        const std::string &getPath() const;
        const std::string &getRoot() const;
        const std::vector<std::string> &getIndex() const;
        const bool &getAutoindex() const;
        const size_t &getClientMaxBodySize() const;
        const std::map<int, std::string> &getErrorPage() const;
        const int &getRedirectCode() const;
        const std::string &getRedirectTarget() const;
        const std::vector<std::string> &getAllowedMethods() const;
        
        // setter
        void setRoot(const std::string &value);
        void setIndex(const std::vector<std::string> &index);
        void setAutoindex(const std::string &value);
        void setClientMaxBodySize(std::string value);
        void setErrorPage(const std::vector<std::string> &values);
        void setRedirection(const std::vector<std::string> &values);
        void setAllowedMethods(const std::vector<std::string> &values);
        void setPath(const std::string &path);

        void addDirectives(const Directive &directive);
};

#endif
