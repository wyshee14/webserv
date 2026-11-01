# include <iostream>
# include "Webserv.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
	{
        std::cerr << "Usage: ./webserv <config_file>" << std::endl;
        return 1;
	}
    std::string path(av[1]);
    try
    {
        Webserv server(path);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
