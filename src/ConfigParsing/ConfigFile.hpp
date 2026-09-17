#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <iostream>
#include <sys/stat.h>   //for stat()
#include <fstream>
# include <vector>

class ConfigFile {
    private:
        std::string _path;

    public:
        // ConfigFile();
        ConfigFile(const std::string &path);
        // ConfigFile(const ConfigFile &copy);
        // ConfigFile &operator=(const ConfigFile &other);
        ~ConfigFile();

        const std::string &getPath() const;
        bool isFileExist() const;
        bool openFile() const;
        std::vector<std::string> readLines() const;
        // remove comments
        // remove whitespace
        void processConfigFile() const;
};

#endif