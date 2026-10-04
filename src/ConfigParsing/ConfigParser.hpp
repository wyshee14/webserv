#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

# include <vector>
# include "Token.hpp"
# include "ServerConfig.hpp"
# include "Directive.hpp"

class ConfigParser {
    private:
        const std::vector<Token> &_tokens;
        std::vector<Token>::const_iterator _current;
        std::vector<Token>::const_iterator _end;

    public:
        ConfigParser(const std::vector<Token> &tokens);
        ~ConfigParser();

        std::vector<ServerConfig> parseTokens();
        ServerConfig parseServerBlock();
        LocationConfig parseLocationBlock();
        Directive parseDirectives();

        // Helper
        bool expectSymbol(TokenType type);

};

#endif