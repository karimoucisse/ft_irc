/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 22:36:20 by marvin            #+#    #+#             */
/*   Updated: 2025/11/03 22:36:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Main.hpp"

class Client
{
public:
	Client(void);
	Client(std::string name, Client &client);
	Client(std::string name);
	Client &operator=(const std::string &name);
	void setUser(std::string user);
	void setNick(std::string user);
	void setPass(std::string user);
	std::string getUser(void) const;
	std::string getNick(void) const;
	std::string getPass(void) const;
	bool isAuth();
	~Client();

private:
	std::string _user;
	std::string _nick;
};
