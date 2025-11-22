#include "Main.hpp"

Server::Server(void)
{}

const std::vector<Channel> &Server::getChannels() const
{
	return _channels;
}

Channel *Server::getOneChannel(std::string name)
{
	std::vector<Channel>::iterator it = _channels.begin();
	std::vector<Channel>::iterator ite = _channels.end();
	while(it != ite)
	{
		if(it->getName() == name)
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
	while(it != ite)
	{
		if(it->getName() == name)
			_channels.erase(it);
		it++;
	}
}
Client *Server::getOneClient(int  fd)
{
	std::vector<Client>::iterator it = _clients.begin();
	std::vector<Client>::iterator ite = _clients.end();
	while(it != ite)
	{
		if(it->getFd() == fd)
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
	while(it != ite)
	{
		if(it->getFd() == fd)
			_clients.erase(it);
		it++;
	}
}
Server::~Server()
{}
