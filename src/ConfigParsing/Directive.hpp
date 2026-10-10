#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include <string>
# include <vector>
# include <stdint.h>    // Library for uint16_t

struct Directive {
    std::string key;
    std::vector<std::string> values;

    Directive(const std::string &key, const std::vector<std::string> &values)
        : key(key), values(values){}
};

enum DirectiveContext
{
    SERVER,
    LOCATION,
    ALL
};

// function pointer (*DirectiveValidator = pointer to a function)
typedef void (*DirectiveValidator)(const std::vector<std::string> &values);

struct DirectiveRule
{
    DirectiveContext context;
    DirectiveValidator validate;

    DirectiveRule(DirectiveContext dContext, DirectiveValidator function)
        : context(dContext), validate(function) {}
};

void validateDirective(const Directive &directive, DirectiveContext context);

struct ListenDirective
{
    std::string address;    // IPV4 format, 4 numbers separated by dots
    uint16_t port;          // port must be number from 0-65535

    ListenDirective(const std::string &address, uint16_t port) : address(address), port(port) {}
};

#endif