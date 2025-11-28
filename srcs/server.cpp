#include "Irc.hpp"

bool Server::_signal = false;
void Server::setSignal(bool b)
{
	_signal = b;
}

Server::Server(int port, std::string password) : _port(port), _password(password), _serverFd(-1), _epollFd(-1)
{
}

void Server::_setSocket(void)
{
	int opt = 1;
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
	{
		std::cerr << "Failed to create socket." << std::endl;
		return;
	}
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_port);
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cerr << "Failed setsockopt." << std::endl;
		return;
	}

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Failed fcntl." << std::endl;
		return;
	}

	if (bind(_serverFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
	{
		std::cerr << "Failed to bind socket." << std::endl;
		return;
	}

	if (listen(_serverFd, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen." << std::endl;
		return;
	}

	_event.events = EPOLLIN;
	_event.data.fd = _serverFd;
	_epollFd = epoll_create1(0);

	if (_epollFd == -1)
	{
		std::cerr << "Failed to create epollFd." << std::endl;
		return;
	}
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &_event) == -1)
	{
		std::cerr << "Failed to add server socket to epoll instance." << std::endl;
		return;
	}
	std::cout << "Server is running on port " << _port << std::endl;
}

void Server::_handleNewClient(void)
{

	Client newClient;
	memset(&_clientAddress, 0, sizeof(_clientAddress));
	socklen_t clientAddressLength = sizeof(_clientAddress);

	int clientFd = accept(_serverFd, (struct sockaddr *)&_clientAddress, &clientAddressLength);
	newClient.setFd(clientFd);
	if (clientFd == -1)
	{
		std::cerr << "Failed to accept client connection." << std::endl;
		return;
	}
	_event.events = EPOLLIN;
	_event.data.fd = clientFd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &_event) == -1)
	{
		std::cerr << "Failed to add client socket to epoll instance." << std::endl;
		close(clientFd);
		return;
	}
	_clients.push_back(newClient);
}

void Server::_handleCmd(std::string str, Client *client)
{
	std::vector<std::string> cmd = splitCmd(str);
	size_t found = str.find_first_not_of(" \t\v");
	if (found != std::string::npos)
		str = str.substr(found);
	if(str.length() < 1)
		return;
	if (str[0] != '/')
		client->send("Command must begin with /, ex: /JOIN or /PASS\n");
	if(cmd[0] == "/pass")
		cmd_pass(cmd, client);
	// if(cmd[0] == "/user")
	// if(cmd[0] == "/nick")
	// if(cmd[0] == "/join")
	// if(cmd[0] == "/invite")
	// if(cmd[0] == "/kick")
	// if(cmd[0] == "/mode")
	// if(cmd[0] == "/topic")
	// if(cmd[0] == "/quit")
}

void Server::_handleClientData(int fd)
{
	Client *client = getOneClient(fd);
	std::vector<std::string> cmd;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);
	if (bytes <= 0)
		return;
	else
	{
		client->setBuffer(buff);
		if (client->getBuffer().find_first_not_of("\r\n") == std::string::npos)
			return;
		cmd = splitCmdLine(client->getBuffer());
		for (size_t i = 0; i < cmd.size(); i++)
			_handleCmd(cmd[i], client);
		if(getOneClient(fd))
			client->clearBuffer();
	}
}

void Server::initServer()
{
	_setSocket();
	while (Server::_signal == false)
	{
		int numEvents = epoll_wait(_epollFd, _events, MAX_EVENTS, -1);
		if (numEvents == -1)
		{
			std::cerr << "Failed to wait for events." << std::endl;
			break;
		}
		for (int i = 0; i < numEvents; ++i)
		{
			if (_events[i].data.fd == _serverFd)
				_handleNewClient();
			else
				_handleClientData(_events[i].data.fd);
		}
	}
}

const std::vector<Channel> &Server::getChannels() const
{
	return _channels;
}

Channel *Server::getOneChannel(std::string name)
{
	std::vector<Channel>::iterator it = _channels.begin();
	std::vector<Channel>::iterator ite = _channels.end();
	while (it != ite)
	{
		if (it->getName() == name)
			return &(*it);
		it++;
	}
	return NULL;
}
void Server::addChannel(Channel &channel)
{
	_channels.push_back(channel);
}
void Server::deleteChannel(std::string name)
{
	std::vector<Channel>::iterator it = _channels.begin();
	std::vector<Channel>::iterator ite = _channels.end();
	while (it != ite)
	{
		if (it->getName() == name)
			_channels.erase(it);
		it++;
	}
}

Client *Server::getOneClient(std::string nick)
{
	std::vector<Client>::iterator it = _clients.begin();
	std::vector<Client>::iterator ite = _clients.end();
	while (it != ite)
	{
		if (it->getNick() == nick)
			return &(*it);
		it++;
	}
	return NULL;
}
Client *Server::getOneClient(int fd)
{
	std::vector<Client>::iterator it = _clients.begin();
	std::vector<Client>::iterator ite = _clients.end();
	while (it != ite)
	{
		if (it->getFd() == fd)
			return &(*it);
		it++;
	}
	return NULL;
}

const std::vector<Client> &Server::getClients() const
{
	return _clients;
}

void Server::addClient(Client &client)
{
	_clients.push_back(client);
}
void Server::deleteClient(int fd)
{
	std::vector<Client>::iterator it = _clients.begin();
	std::vector<Client>::iterator ite = _clients.end();
	while (it != ite)
	{
		if (it->getFd() == fd)
			_clients.erase(it);
		it++;
	}
}
Server::~Server()
{
}
