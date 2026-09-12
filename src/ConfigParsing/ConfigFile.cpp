# include "ConfigFile.hpp"

ConfigFile::ConfigFile(const std::string &path) : _path(path){}

ConfigFile::~ConfigFile(){}

const std::string &ConfigFile::getPath() const
{
    return _path;
}

bool ConfigFile::isFileExist() const
{
    // Structure which would store metadata
    struct stat sb;

    if (stat(_path.c_str(), &sb) == 0)
    {
        std::cout << " Path is valid" << std::endl;
        return 1;
    }
    std::cout << "Path is invalid" << std::endl;
    return 0;
}

void ConfigFile::validateFilePath() const
{
    if (!isFileExist())
        return ;
}