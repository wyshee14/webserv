# include "ConfigFile.hpp"
# include <sys/stat.h>
# include <fstream>
# include <iostream>
# include <stdexcept>
# include <cctype>
# include "ConfigException.hpp"

ConfigFile::ConfigFile(const std::string &path) : _path(path), _tokens() {}

ConfigFile::~ConfigFile(){}

const std::string &ConfigFile::getPath() const
{
    return _path;
}

const std::vector<Token> &ConfigFile::getTokens() const
{
    return _tokens;
}

bool ConfigFile::isFileExist() const
{
    // Structure which would store metadata
    struct stat fileInfo;

    // If the file path/directory exists at the path returns 0
    if (stat(_path.c_str(), &fileInfo) != 0) 
        return false;
    // returns boolean on path is a regular file; 0 is false(means is directory or others)
    if (!S_ISREG(fileInfo.st_mode))
        return false;
    // check if the file ends with .conf
    if (_path.length() < 5 || _path.substr(_path.length() - 5) != ".conf")
        return false;
    return true;
}

bool ConfigFile::openFile() const
{
    std::ifstream file(_path.c_str());
    if (!file.is_open())
    {
        std::cout << "Configuration file cannot be opened" << std::endl;
        return false;
    }
    return true;
}

void ConfigFile::readLines()
{
    std::string line;
    std::ifstream config(_path.c_str());
    while (std::getline(config, line))
    {
        removeComments(line);
        tokenize(line);
    }
    config.close();
}

void ConfigFile::removeComments(std::string &line)
{
    size_t found = line.find("#");
    if (found != std::string::npos)
    {
        line.erase(found);
    }
}

void ConfigFile::printTokens() const
{
    for (std::vector<Token>::const_iterator it = _tokens.begin(); it != _tokens.end(); it++)
    {
        std::cout << "Token type: " << (*it).type << ", value: " << (*it).value << std::endl;
    }
}

static bool isSymbol(char c)
{
    if (c == '{' || c == '}' || c == ';')
        return true;
    return false;
}

void ConfigFile::tokenize(std::string &line)
{
    std::string word;
    for(std::string::const_iterator it = line.begin(); it != line.end(); it++)
    {
        if (std::isspace(*it) || isSymbol(*it))
        {
            if (!word.empty())
            {
                _tokens.push_back(Token(WORD, word));
                word.clear();
            }
            if (isSymbol(*it))
            {
                TokenType type;
                if (*it == '{')
                    type = LBRACES;
                else if (*it == '}')
                    type = RBRACES;
                else
                    type = SEMICOLON;
                _tokens.push_back(Token (type, std::string(1, *it)));
            }
        }
        else
            word.push_back(*it);
    }
    if (!word.empty())
    {
        _tokens.push_back(Token(WORD, word));
        word.clear();
    }
}

void ConfigFile::processConfigFile()
{
    if (_path.empty())
        throw std::runtime_error("Configuration path is empty");
    if (!isFileExist())
        throw (ConfigException(ConfigException::INVALID_FILE, getPath()));
        // throw std::runtime_error("Configuration path is missing or is not a regular file: " + _path);
    if (!openFile())
        throw std::runtime_error("Configuration file failed to open");
    readLines();
    printTokens();
}