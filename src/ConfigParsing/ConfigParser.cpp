# include "ConfigParser.hpp"
# include "ServerConfig.hpp"
# include <stdexcept>
# include <iostream>
# include "ConfigException.hpp"
# include "Directive.hpp"
# include "ConfigParser.hpp"

ConfigParser::ConfigParser(){}

ConfigParser::~ConfigParser() {}

void ConfigParser::parseTokens(std::vector<Token> tokens)
{
    std::vector<ServerConfig> servers;     // an array of server block
    std::vector<Token>::iterator current = tokens.begin();

    // check if the token is nothing
    if (current == tokens.end())
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    while (current != tokens.end())
    {
        // check the first word must be server
        if (current->type != WORD || current->value != "server")
        {
            std::cout << "currenttt: [" << current->value << "]" << std::endl;
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
        }
        servers.push_back(parseServerBlock(current, tokens.end()));
    }
}

ServerConfig ConfigParser::parseServerBlock(std::vector<Token>::iterator &current, std::vector<Token>::iterator end)
{
    ServerConfig server;

    // server
    if (current == end || current->value != "server")
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    ++current;

    // '{'
    if (current == end || current->type != LBRACES)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    ++current;
    
    while (current != end && current->type != RBRACES)
    {
        if (current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
        if (current->value == "location")
        {
            // LocationConfig location = parseLocationBlock(current, end);
            // server.addLocation(location);
        }
        else
        {
            Directive directive = parseDirectives(current, end);
            validateDirective(directive, SERVER);
            server.addDirectives(directive);
        }
    }
    if (current == end || current->type != RBRACES)
    {
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    }
    // skip the close bracket '}'
    ++current;
    return server;
}

Directive ConfigParser::parseDirectives(std::vector<Token>::iterator &current, std::vector<Token>::iterator end) 
{
    if (current == end || current->type != WORD)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    std::string key = current->value;
    std::vector<std::string> values;
    // skip the key
    ++current;
    while (current != end && current->type == WORD)
    {
        values.push_back(current->value);
        ++current;
    }
    if (current == end || current->type != SEMICOLON)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    // skip semicolon
    ++current;
    return Directive(key, values);
}
