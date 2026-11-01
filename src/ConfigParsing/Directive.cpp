# include <iostream>
# include <map>
# include <stdlib.h>    // library for strtoul
# include <sys/stat.h>
# include "Directive.hpp"
# include "ConfigException.hpp"
# include "ConfigFile.hpp"

static bool isAllDigit(const std::string &raw, unsigned long max)
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

// http part will append with the root and check if its a valid file
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
    if (values[0] != "on" && values[0] != "off")
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[0]);
}

// [status_code](n) [error_file](1)
static void validateErrorPage(const std::vector<std::string> &values)
{
    std::vector<std::string> statusCode;
    if (values.empty())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "error_page");
    // exclude the last one for file
    for (size_t i = 0; i < values.size() - 1; i++)
    {
        for(size_t j = 0; j < values[i].length(); j++)
        {
            if (!std::isdigit(values[i][j]))
                break ;
        }
        unsigned long status = strtoul(values[i].c_str(), NULL, 10);
        if (status < 100 || status > 599)
            throw ConfigException(ConfigException::INVALID_STATUS_CODE, values[i]);
        statusCode.push_back(values[i]);
    }
    if (values.size() - statusCode.size() != 1)
        throw ConfigException(ConfigException::MISCONFIGURED_ERROR_FILE);
}

// Default : 1M
// Last character can be K, G, M, or none(represents in bytes)
// other all digits
static void validateClientMaxBodySize(const std::vector<std::string> &values)
{
    if (values.size() != 1 || values[0].empty())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "client_max_body_size");
    const std::string &value = values[0];
    size_t digitCount = 0;
    // cast converts to a positive byte value - prevent nagtive char value
    while (digitCount < value.size() && std::isdigit(static_cast<unsigned char>(value[digitCount])))
        ++digitCount;
    if (digitCount == 0)
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, value);
    if (digitCount < value.size())
    {
        char suffix = value[digitCount];
        if (suffix != 'K' && suffix != 'M' && suffix != 'G') 
            throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, value);
        ++digitCount;
    }
    if (digitCount != value.size())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, value);
}

static void validateMethods(const std::vector<std::string> &values)
{
    if (values.empty())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "methods");
    for (size_t i = 0; i < values.size(); i++)
    {
        if (values[i] != "GET" && values[i] != "POST" && values[i] != "DELETE")
            throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, values[i]);
    }
}

// [code] [path/url] 
static void validateReturn(const std::vector<std::string> &values)
{
    if (values.size() != 2 || values[0].empty() || values[1].empty())
        throw ConfigException(ConfigException::INVALID_DIRECTIVE_VALUES, "return");
    // check status code
    for(size_t i = 0; i < values[0].length(); i++)
    {
        if (!std::isdigit(values[0][i]))
            throw ConfigException(ConfigException::INVALID_STATUS_CODE, values[0]);
    }
    unsigned long status = strtoul(values[0].c_str(), NULL, 10);
    if (status < 300 || status > 308)
        throw ConfigException(ConfigException::INVALID_STATUS_CODE, values[0]);
    // check path
    std::string path = values[1];
    if (path[0] != '/')
        throw ConfigException(ConfigException::INVALID_REDIRECT_PATH, path);
    if (path.find_first_of(";,") != std::string::npos)
		throw (ConfigException(ConfigException::INVALID_LOCATION_PATH, path));
}

typedef std::map<std::string, DirectiveRule> DirectiveMap;

static DirectiveMap createDirectiveRules()
{
    DirectiveMap rules;

    rules.insert(std::make_pair("listen", DirectiveRule(SERVER, validateListen)));
    rules.insert(std::make_pair("allowed_methods", DirectiveRule(LOCATION, validateMethods)));
    rules.insert(std::make_pair("root", DirectiveRule(ALL, validateRoot)));
    rules.insert(std::make_pair("index", DirectiveRule(ALL, validateIndex)));
    rules.insert(std::make_pair("autoindex", DirectiveRule(ALL, validateAutoindex)));
    rules.insert(std::make_pair("error_page", DirectiveRule(ALL, validateErrorPage)));
    rules.insert(std::make_pair("client_max_body_size", DirectiveRule(ALL, validateClientMaxBodySize)));
    // return is available in all blocks, but for this project set explicitly to location only
    rules.insert(std::make_pair("return", DirectiveRule(LOCATION, validateReturn)));

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