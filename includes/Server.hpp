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
	const std::vector<Client> &getClients() const;
	void addClient(Client &client);
	void deleteClient(int fd);
	~Server();
private:
	std::vector<Channel> _channels;
	std::vector<Client> _clients;
	std::vector<Client> _ncClients;
};
