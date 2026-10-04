# include "ConfigParser.hpp"
# include "ServerConfig.hpp"
# include <stdexcept>
# include <iostream>
# include "ConfigException.hpp"
# include "Directive.hpp"
# include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::vector<Token> &tokens) 
    : _tokens(tokens), _current(_tokens.begin()), _end(_tokens.end()){}

ConfigParser::~ConfigParser() {}

std::vector<ServerConfig> ConfigParser::parseTokens()
{
    std::vector<ServerConfig> servers;     // an array of server block

    // check if the token is nothing
    if (_current == _end)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    while (_current != _end)
    {
        // check the first word must be server
        if (_current->type != WORD || _current->value != "server")
        {
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
        }
        servers.push_back(parseServerBlock());
    }
    return servers;
}

ServerConfig ConfigParser::parseServerBlock()
{
    ServerConfig server;

    // skip "server"
    ++_current;

    if (!expectSymbol(LBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    
    while (_current != _end && _current->type != RBRACES)
    {
        if (_current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
        if (_current->value == "location")
        {
            LocationConfig location = parseLocationBlock();
            server.addLocation(location);
        }
        else
        {
            Directive directive = parseDirectives();
            validateDirective(directive, SERVER);
            server.addDirectives(directive);
        }
    }
    if (!expectSymbol(RBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    return server;
}

LocationConfig ConfigParser::parseLocationBlock()
{
    LocationConfig location;

    // skip "location"
    ++_current;

    // location must have path
    if (_current != _end && _current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    location.validatePath(_current->value);
    ++_current;

    if (!expectSymbol(LBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    
    while (_current != _end && _current->type != RBRACES)
    {
        if (_current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
        else
        {
            Directive directive = parseDirectives();
            validateDirective(directive, LOCATION);
            location.addDirectives(directive);
        }
    }
    if (!expectSymbol(RBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    return location;
}

Directive ConfigParser::parseDirectives() 
{
    if (_current == _end || _current->type != WORD)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    std::string key = _current->value;
    std::vector<std::string> values;
    // skip the key
    ++_current;
    while (_current != _end && _current->type == WORD)
    {
        values.push_back(_current->value);
        ++_current;
    }
    if (!expectSymbol(SEMICOLON))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    return Directive(key, values);
}

bool ConfigParser::expectSymbol(TokenType type)
{
    if (_current == _end || _current->type != type)
        return false;
    // skip the braces
    ++_current;
    return true;
}