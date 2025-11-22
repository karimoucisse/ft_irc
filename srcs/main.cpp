#include "Main.hpp"

void parseAuthCmd(Client *c, std::string line, Server &server)
{
    std::cout << line << std::endl;
    std::stringstream ss(line);
    std::string cmd, arg;
    ss >> cmd >> arg;

    for (char &x : cmd)
        x = toupper(x);

    if (cmd == "PASS")
    {
        c->setPass(arg);
        std::cout << "PASS = " << c->getPass() << std::endl;
    }
    else if (cmd == "NICK")
    {
        c->setNick(arg);
        std::cout << "NICK = " << c->getNick() << std::endl;
    }
    else if (cmd == "USER")
    {
        c->setUser(arg);
        std::cout << "USER = " << c->getUser() << std::endl;
    }
    if (c->isAuth())
        std::cout << "Welcome !!" << std::endl;
}

void parseCmd(Client *c, std::string line, Server &server)
{
    std::cout << line << std::endl;
    std::stringstream ss(line);
    std::string cmd, arg;
    ss >> cmd >> arg;

    for (char &x : cmd)
        x = toupper(x);

    if (cmd == "/JOIN")
    {
        std::cout << line << std::endl;
    }
    if (cmd == "/INVITE")
        std::cout << line << std::endl;
    if (cmd == "/KICK")
        std::cout << line << std::endl;
    if (cmd == "/MODE")
        std::cout << line << std::endl;
    if (cmd == "/QUIT")
        std::cout << line << std::endl;
    if (cmd == "/TOPIC")
        std::cout << line << std::endl;
}

void handleCmd(int clientFd, Server &server)
{
    Client *c = server.getOneClient(clientFd);
    if (!c->isAuth())
        return;
    char buffer[1024];
    int bytes = read(clientFd, buffer, sizeof(buffer));

    if (bytes <= 0)
    {
        server.deleteClient(clientFd);
        close(clientFd);
        return;
    }

    parseCmd(c, buffer, server);
    memset(buffer, 0, sizeof(buffer));
}

void connectClient(int clientFd, Server &server)
{

    Client *c = server.getOneClient(clientFd);
    if (c->isAuth())
        return;
    char buffer[1024];
    int bytes = read(clientFd, buffer, sizeof(buffer));

    if (bytes <= 0)
    {
        server.deleteClient(clientFd);
        close(clientFd);
        return;
    }
    parseAuthCmd(c, buffer, server);
    memset(buffer, 0, sizeof(buffer));
}

int main(int ac, char **av)
{
    if (ac != 3)
        return (std::cout << "Usage: ./ircserv <port> <password>" << std::endl, EXIT_FAILURE);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(av[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int epollFd;
    struct epoll_event event, events[MAX_EVENTS];
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)))
    {
        close(serverSocket);
        return (std::cerr << "Error: Failed to bind socket." << std::endl, EXIT_FAILURE);
    }
    if (listen(serverSocket, MAX_CLIENTS) == -1)
    {
        std::cerr << "Failed to listen." << std::endl;
        close(serverSocket);
        return 1;
    }

    epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        std::cerr << "Failed to create epoll instance." << std::endl;
        close(serverSocket);
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1)
    {
        std::cerr << "Failed to add server socket to epoll instance." << std::endl;
        close(serverSocket);
        close(epollFd);
        return 1;
    }

    std::cout << "Server started. Listening on port " << atoi(av[1]) << std::endl;
    Server server;
    while (1)
    {
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (numEvents == -1)
        {
            std::cerr << "Failed to wait for events." << std::endl;
            break;
        }
        for (int i = 0; i < numEvents; ++i)
        {
            if (events[i].data.fd == serverSocket)
            {
                sockaddr_in clientAddress;
                socklen_t clientAddressLength = sizeof(clientAddress);
                int clientFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
                if (clientFd == -1)
                {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
                }
                event.events = EPOLLIN;
                event.data.fd = clientFd;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1)
                {
                    std::cerr << "Failed to add client socket to epoll instance." << std::endl;
                    close(clientFd);
                    continue;
                }
                Client client(clientFd);
                server.addClient(client);
                connectClient(clientFd, server);
            }
            else
            {
                int clientFd = events[i].data.fd;
                connectClient(clientFd, server);
                handleCmd(clientFd, server);
            }
        }
    }
    close(serverSocket);
    close(epollFd);
    return 0;
}
