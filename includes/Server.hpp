/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:17:56 by marvin            #+#    #+#             */
/*   Updated: 2025/11/03 22:17:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Irc.hpp"

class Client;
class Server
{
public:
	Server(int port, std::string password);
	void initServer(void);
	static void setSignal(bool b);
	~Server();
	Channel *getOneChannel(std::string name);
	const std::vector<Channel> &getChannels() const;
	void addChannel(Channel &channel);
	void deleteChannel(std::string name);
	Client *getOneClient(int fd);
	Client *getOneClient(std::string nick);
	const std::vector<Client> &getClients() const;
	void addClient(Client &client);
	void deleteClient(int fd);

	void cmd_pass(std::vector<std::string> cmd, Client *client);

private:
	int _port;
	int _serverFd;
	int _epollFd;
	std::string _password;
	static bool _signal;
	struct sockaddr_in _serverAddress;
	struct sockaddr_in _clientAddress;
	struct epoll_event _event;
	struct epoll_event _events[MAX_EVENTS];
	void _setSocket(void);
	void _handleNewClient(void);
	void _handleClientData(int fd);
	void _handleCmd(std::string str, Client *client);
	std::vector<Channel> _channels;
	std::vector<Client> _clients;
};
