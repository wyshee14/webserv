#ifndef REQUESTCONFIG_HPP
# define REQUESTCONFIG_HPP

# include <iostream>
# include <vector>

// to be pass to http request a based on client request path
class RequestConfig {
    private:
        std::string _path;

    public:
        RequestConfig();
        ~RequestConfig();
};

#endif
