#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <iostream>
#include <sys/stat.h>   //for stat()
#include <fstream>
# include <vector>

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

class ConfigFile {
    private:
        const std::string _path;
        std::vector<Token> _tokens;

    public:
        ConfigFile(const std::string &path);
        ~ConfigFile();

        const std::string &getPath() const;
        const std::vector<Token> &getTokens() const;
        void printTokens() const;
        bool isFileExist() const;
        bool openFile() const;
        void readLines();
        void removeComments(std::string &line);
        void tokenize(std::string &line);
        void processConfigFile();
};

#endif