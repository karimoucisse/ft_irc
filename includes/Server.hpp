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

#include "Main.hpp"

class Client;
class Server
{
public:
	Server(void);
	Channel *getOneChannel(std::string name);
	const std::vector<Channel> &getChannels() const;
	void addChannel(Channel &channel);
	void deleteChannel(std::string name);
	Client *getOneClient(int fd);
	Client *getOneClient(std::string nick);
	const std::vector<Client> &getClients() const;
	void addClient(Client &client);
	void deleteClient(int fd);
	~Server();
	void join(std::string cmd, int fd);
	void topic(std::string cmd, int fd);
	void kick(std::string cmd, int fd);

private:
	std::vector<Channel> _channels;
	std::vector<Client> _clients;
	std::vector<Client> _ncClients;
};
