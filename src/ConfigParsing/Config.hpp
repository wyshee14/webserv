#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <vector>

// to be pass to http request a based on client request path
class Config {
    private:
        std::string _path;

    public:
        Config();
        ~Config();
};

#endif
