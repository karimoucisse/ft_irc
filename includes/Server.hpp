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

class Server
{
public:
	Server(void);
	Server(std::string name, Client &client);
	Server(std::string name);
	Server &operator=(const std::string &name);
	~Server();
private:
	std::vector<Channel> _channels;
	std::vector<Client> _clients;
};
