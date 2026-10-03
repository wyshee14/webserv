# include "Directive.hpp"
# include <map>
# include "ConfigException.hpp"

static void validateListen(const std::vector<std::string> &values)
{
    if (values.size() != 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateServerName(const std::vector<std::string> &values)
{
    if (values.size() != 1)
        throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateRoot(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateIndex(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateAutoindex(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateErrorPage(const std::vector<std::string> &values)
{
    if (values.size() < 1)
        throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
    }

    static void validateClientMaxBodySize(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateMethods(const std::vector<std::string> &values)
{
    if (values.size() < 1)
    throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
}

static void validateReturn(const std::vector<std::string> &values)
{
    if (values.size() < 1)
        throw ConfigException(ConfigException::INVLAID_DIRECTIVE_VALUES);
    }

typedef std::map<std::string, DirectiveRule> DirectiveMap;

static DirectiveMap createDirectiveRules()
{
    DirectiveMap rules;

    rules.insert(std::make_pair("listen", DirectiveRule(SERVER, validateListen)));
    rules.insert(std::make_pair("server_name", DirectiveRule(SERVER, validateServerName)));
    rules.insert(std::make_pair("root", DirectiveRule(ALL, validateRoot)));
    rules.insert(std::make_pair("index", DirectiveRule(ALL, validateIndex)));
    rules.insert(std::make_pair("autoindex", DirectiveRule(ALL, validateAutoindex)));
    rules.insert(std::make_pair("error_page", DirectiveRule(ALL, validateErrorPage)));
    rules.insert(std::make_pair("client_max_body_size", DirectiveRule(ALL, validateClientMaxBodySize)));
    rules.insert(std::make_pair("methods", DirectiveRule(LOCATION, validateMethods)));
    rules.insert(std::make_pair("return", DirectiveRule(ALL, validateReturn)));

    return rules;
}

void validateDirective(const Directive &directive, DirectiveContext context)
{
    static DirectiveMap rules = createDirectiveRules();
    DirectiveMap::iterator it = rules.find(directive.key);
    if (it == rules.end())
        throw ConfigException(ConfigException::UNKNOWN_DIRECTIVE);
    // get thevalue of the key found
    const DirectiveRule &found = it->second;
    if (found.context != ALL && found.context != context)
        throw ConfigException(ConfigException::WRONG_DIRECTIVE_CONTEXT);
    found.validate(directive.values);
}