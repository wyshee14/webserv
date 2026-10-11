# include "Directive.hpp"
# include <map>
# include "ConfigException.hpp"
# include <stdlib.h>    // library for strtoul
# include <sys/stat.h>
# include "ConfigFile.hpp"
# include <iostream>

static bool isAllDigit(std::string &raw, unsigned long max)
{
    for(size_t i = 0; i < raw.length(); i++)
    {
        if (!std::isdigit(raw[i]))
            return false;
    }
    unsigned long number = strtoul(raw.c_str(), NULL, 10);
    if (number > max)
        return false;
    return true;
}

// define interface:port pairs - can have multiple values
static void validateListen(const std::vector<std::string> &values)
{
    if (values.size() != 1)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "listen");
    std::string address;
    std::string port;
    size_t separator = values[0].find(':');
    if (separator == std::string::npos)
    {
        address = "0.0.0.0";
        port = values[0];
    }
    else 
    {    
        address = values[0].substr(0, separator);
        port = values[0].substr(separator + 1);
    }
    if (port.empty())
        throw ConfigException(ConfigException::INVALID_PORT_NUMBER);
    std::vector<std::string> octets;
    size_t start = 0;
    size_t dot;
    while ((dot = address.find('.', start)) != std::string::npos)
    {
        octets.push_back(address.substr(start, dot-start));
        start = dot + 1;
    }
    // process the last octet
    octets.push_back(address.substr(start));
    if (octets.size() != 4)
        throw ConfigException(ConfigException::INVALID_IP_ADDRESS, address);
    for (size_t i = 0; i < octets.size(); ++i)
    {
        if (octets[i].empty())
            throw ConfigException(ConfigException::INVALID_IP_ADDRESS, address);
        if (!isAllDigit(octets[i], 255))
            throw ConfigException(ConfigException::INVALID_IP_ADDRESS, address);
    }
    if (!isAllDigit(port, 65535))
        throw ConfigException(ConfigException::INVALID_PORT_NUMBER, port);
}

// accepts exactly one path
static void validateRoot(const std::vector<std::string> &values)
{
    if (values.size() != 1)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "root");
    
    struct stat fileInfo;

    // If the file path/directory exists at the path returns 0
    if (stat(values[0].c_str(), &fileInfo) != 0) 
        throw ConfigException(ConfigException::INVALID_DIRECTORY, values[0]);
    // check if path exists but is not a directory
    if (!S_ISDIR(fileInfo.st_mode))
        throw ConfigException(ConfigException::INVALID_DIRECTORY, values[0]);
}

// cehck that this should be a file
static void validateIndex(const std::vector<std::string> &values)
{
    if (values.empty())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "index");
    for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        if (it->empty() || it->find('/') != std::string::npos)
            throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, *it);
    }
}

static void validateAutoindex(const std::vector<std::string> &values)
{
    if (values.size() != 1)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "autoindex");
    if (values[0] != "true" && values[0] != "false")
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "autoindex");
}

static void validateErrorPage(const std::vector<std::string> &values)
{
    if (values.size() < 1)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[0]);
    }

    static void validateClientMaxBodySize(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[0]);
}

static void validateMethods(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[0]);
}

static void validateReturn(const std::vector<std::string> &values)
{
    if (values.size() < 1)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[0]);
    }

typedef std::map<std::string, DirectiveRule> DirectiveMap;

static DirectiveMap createDirectiveRules()
{
    DirectiveMap rules;

    rules.insert(std::make_pair("listen", DirectiveRule(SERVER, validateListen)));
    rules.insert(std::make_pair("methods", DirectiveRule(LOCATION, validateMethods)));
    rules.insert(std::make_pair("root", DirectiveRule(ALL, validateRoot)));
    rules.insert(std::make_pair("index", DirectiveRule(ALL, validateIndex)));
    rules.insert(std::make_pair("autoindex", DirectiveRule(ALL, validateAutoindex)));
    rules.insert(std::make_pair("error_page", DirectiveRule(ALL, validateErrorPage)));
    rules.insert(std::make_pair("client_max_body_size", DirectiveRule(ALL, validateClientMaxBodySize)));
    rules.insert(std::make_pair("return", DirectiveRule(ALL, validateReturn)));

    return rules;
}

void validateDirective(const Directive &directive, DirectiveContext context)
{
    static DirectiveMap rules = createDirectiveRules();
    DirectiveMap::iterator it = rules.find(directive.key);
    if (it == rules.end())
        throw ConfigException(ConfigException::UNKNOWN_DIRECTIVE, directive.key);
    // get the rule value of the key found
    const DirectiveRule &found = it->second;
    // check the directive context is following the rule
    if (found.context != ALL && found.context != context)
        throw ConfigException(ConfigException::WRONG_DIRECTIVE_CONTEXT, directive.key);
    found.validate(directive.values);
}