#include "Main.hpp"

void handleClient(int clientFd)
{
    char buffer[1024];
    std::string pass = "";
    std::string nick = "";
    std::string user = "";
    while (true)
    {
        int bytesRead = read(clientFd, buffer, sizeof(buffer));
        if (bytesRead <= 0)
        {
            break;
        }
        char cmd1[256];
        char cmd2[256];
        if (sscanf(buffer, "%s %s", cmd1, cmd2) != 2)
            continue;
        std::string c1(cmd1);
        std::string c2(cmd2);

        if ((c1 == "pass" || c1 == "PASS") && pass.empty())
            pass = c2;
        if ((c1 == "nick" || c1 == "NICK") && nick.empty())
            nick = c2;
        if ((c1 == "user" || c1 == "USER") && user.empty())
            user = c2;
        if (!user.empty() && !pass.empty() && !nick.empty())
            break;
        // write(clientFd, buffer, bytesRead);
    }

    std::cout << nick << " is connected !" << std::endl;
    close(clientFd);
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

                std::thread clientThread(handleClient, clientFd);
                clientThread.detach();
            }
            else
            {
                int clientFd = events[i].data.fd;
                std::thread clientThread(handleClient, clientFd);
                clientThread.detach();
            }
        }
    }
    close(serverSocket);
    close(epollFd);
    return 0;
}
