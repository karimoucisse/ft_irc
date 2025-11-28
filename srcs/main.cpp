#include "Irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cout << RED << "Usage: ./ircserv <port> <password>" << RESET << std::endl, EXIT_FAILURE);
    if (!checkPort(av[1]))
        return 1;
    Server server(std::atoi(av[1]), av[2]);
    try
    {
        // handle Signals
        server.initServer();
    }
    catch (const std::exception &e)
    {
        // server.deleteAllClients();
        std::cerr << e.what() << '\n';
    }
    std::cout << "Server closed !" << std::endl;
    return 0;
}
