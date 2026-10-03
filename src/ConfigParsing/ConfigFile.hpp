#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <string>
# include <vector>
# include <sys/stat.h>   //for stat()
# include <fstream>
# include "Token.hpp"

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