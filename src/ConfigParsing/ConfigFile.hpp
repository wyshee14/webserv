#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

# include <iostream>
#include <sys/stat.h>   //for stat()
#include <fstream>

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
        void readConfigFile() const;
        // readFiles
        // remove comments
        // remove whitespace
};

#endif