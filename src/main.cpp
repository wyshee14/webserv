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
		int toProceed = factory.SocketBuilder();
        if (toProceed == 0)
        {
            Multiplexer event(factory.getlsocs());
            try {
                event.eventLoop();
            }
            catch (std::exception& e) {
    
            }
        }
        else
            return(1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
