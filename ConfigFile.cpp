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
    struct stat fileInfo;

    // If the file path/directory exists at the path returns 0
    if (stat(_path.c_str(), &fileInfo) != 0) 
    {
        std::cout << " Path is invalid" << std::endl;
        return false;
    }
    std::cout << "Path is valid" << std::endl;
    // returns boolean on path is a regular file; 0 is false(means is directory or others)
    if (!S_ISREG(fileInfo.st_mode))
    {
        std::cout << "Path is not a regular file" << std::endl;
        return false;
    }
    // check if the file ends with .conf
    if (_path.length() < 5 || _path.substr(_path.length() - 5) != ".conf")
        return false;
    std::cout << "Path is a valid configuration file" << std::endl;
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

std::vector<std::string> ConfigFile::readLines()
{
    std::string line;
    std::ifstream config(_path.c_str());
    while (getline(config, line))
    {
        std::cout << line << std::endl;
    }
    config.close();

}

void ConfigFile::processConfigFile() const
{
    // check path is empty
    if (_path.empty())
        throw std::runtime_error("Configuration path is empty") ;
    // check file exists
    if (!isFileExist())
        throw std::runtime_error("Configuration path is missing or is not a regular file: " + _path);
    // open file
    if (!openFile())
        throw std::runtime_error("Configuration file failed to open");
    // read lines
    // remove comments and whitespaces
    // tokenize directives
    // build ServerConfig and LocationConfig objects
}