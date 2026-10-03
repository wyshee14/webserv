#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

enum TokenType {
    WORD,
    LBRACES,
    RBRACES,
    SEMICOLON
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType tokenType, const std::string &tokenValue)
        : type(tokenType), value(tokenValue) {}
};

#endif
