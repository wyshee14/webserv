#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

# include <vector>
# include "Token.hpp"
# include "ServerConfig.hpp"
# include "Directive.hpp"

// TODO: convert to a free function, no need to be in a class
class ConfigParser {
    public:
        ConfigParser();
        ~ConfigParser();

        void parseTokens(std::vector<Token> tokens);
        ServerConfig parseServerBlock(std::vector<Token>::iterator &current, std::vector<Token>::iterator end);
        Directive parseDirectives(std::vector<Token>::iterator &current, std::vector<Token>::iterator end);
};

#endif