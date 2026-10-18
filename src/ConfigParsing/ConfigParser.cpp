# include "ConfigParser.hpp"
# include "ConfigException.hpp"
# include <stdexcept>
# include <iostream>

ConfigParser::ConfigParser(const std::vector<Token> &tokens) 
    : _tokens(tokens), _current(_tokens.begin()), _end(_tokens.end()){}

ConfigParser::~ConfigParser() {}

std::vector<ServerConfig> ConfigParser::parseTokens()
{
    std::vector<ServerConfig> servers;     // an array of server block

    if (_tokens.empty())
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN);
    // check if the token is nothing
    if (_current == _end)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    while (_current != _end)
    {
        // check the first word must be server
        if (_current->type != WORD || _current->value != "server")
        {
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
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
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    
    while (_current != _end && _current->type != RBRACES)
    {
        if (_current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
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
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    if (server.getListenEndpoint().empty())
         server.setListenEndpoints("8080");
    return server;
}

LocationConfig ConfigParser::parseLocationBlock()
{
    LocationConfig location;

    // skip "location"
    ++_current;

    // location must have path
    if (_current == _end || _current->type != WORD)
            throw ConfigException(
                ConfigException::UNEXPECTED_TOKEN, 
                _current == _end ? "end of file" : _current->value);
    location.setPath(_current->value);
    ++_current;

    if (!expectSymbol(LBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    
    while (_current != _end && _current->type != RBRACES)
    {
        if (_current->type != WORD)
            throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
        else
        {
            Directive directive = parseDirectives();
            validateDirective(directive, LOCATION);
            location.addDirectives(directive);
        }
    }
    if (!expectSymbol(RBRACES))
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    return location;
}

Directive ConfigParser::parseDirectives() 
{
    if (_current == _end || _current->type != WORD)
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
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
        throw ConfigException(ConfigException::UNEXPECTED_TOKEN, _current->value);
    return Directive(key, values);
}

bool ConfigParser::expectSymbol(TokenType type)
{
    if (_current == _end || _current->type != type)
        return false;
    // skip the braces/semicolon
    ++_current;
    return true;
}

// <-----TO BE REMOVED (FOR DEBUGGING PURPOSE ONLY)----->
static void printStringVector(const std::vector<std::string> &values)
{
    for (std::vector<std::string>::const_iterator it = values.begin();
         it != values.end(); ++it)
    {
        std::cout << *it << " ";
    }
}

static void printErrorPages(const std::map<int, std::string> &errorPages,
                            const std::string &indent)
{
    for (std::map<int, std::string>::const_iterator it = errorPages.begin();
         it != errorPages.end(); ++it)
    {
        std::cout << indent << "error_page: "
                  << it->first << " " << it->second << std::endl;
    }
}

static void printLocationConfig(const LocationConfig &location)
{
    std::cout << "    location " << location.getPath() << " {" << std::endl;
    std::cout << "        root: " << location.getRoot() << std::endl;

    std::cout << "        index: ";
    printStringVector(location.getIndex());
    std::cout << std::endl;

    std::cout << "        allowed_methods: ";
    printStringVector(location.getAllowedMethods());
    std::cout << std::endl;

    std::cout << "        autoindex: "
              << (location.getAutoindex() ? "true" : "false")
              << std::endl;

    std::cout << "        client_max_body_size: "
              << location.getClientMaxBodySize()
              << std::endl;

    printErrorPages(location.getErrorPage(), "        ");

    if (!location.getRedirectTarget().empty())
    {
        std::cout << "        redirect: "
                  << location.getRedirectCode() << " "
                  << location.getRedirectTarget()
                  << std::endl;
    }

    std::cout << "    }" << std::endl;
}

void printAllConfiguration(const std::vector<ServerConfig> &servers)
{
    for (std::vector<ServerConfig>::const_iterator serverIt = servers.begin();
         serverIt != servers.end(); ++serverIt)
    {
        const ServerConfig &server = *serverIt;

        std::cout << "server {" << std::endl;

        std::cout << "    listen: ";
        const std::vector<ListenDirective> &listeners =
            server.getListenEndpoint();

        for (std::vector<ListenDirective>::const_iterator listenerIt =
                 listeners.begin();
             listenerIt != listeners.end(); ++listenerIt)
        {
            std::cout << listenerIt->address << ":"
                      << listenerIt->port << " ";
        }
        std::cout << std::endl;

        std::cout << "    root: " << server.getRoot() << std::endl;

        std::cout << "    index: ";
        printStringVector(server.getIndex());
        std::cout << std::endl;

        std::cout << "    autoindex: "
                  << (server.getAutoindex() ? "true" : "false")
                  << std::endl;

        std::cout << "    client_max_body_size: "
                  << server.getClientMaxBodySize()
                  << std::endl;

        printErrorPages(server.getErrorPage(), "    ");

        const std::vector<LocationConfig> &locations =
            server.getLocations();

        for (std::vector<LocationConfig>::const_iterator locationIt =
                 locations.begin();
             locationIt != locations.end(); ++locationIt)
        {
            printLocationConfig(*locationIt);
        }

        std::cout << "}" << std::endl;
    }
}