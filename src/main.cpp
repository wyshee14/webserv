# include <iostream>
# include "Webserv.hpp"
# include "EventLoop/MasterProcess.hpp"

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
		MasterProcess factory(server.getServers());
		if (factory.SocketBuilder() == 1)
            return 1;
        try {
            Multiplexer event(factory.getlsocs());
            event.eventLoop();
        }
        catch (std::exception& e) {
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
