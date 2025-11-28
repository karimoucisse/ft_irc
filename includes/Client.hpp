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
#include "Irc.hpp"

class Client
{
public:
	Client(void);
	Client(int fd);
	Client(std::string p, std::string n, std::string u);
	Client(Client const &client);
	Client &operator=(Client const &client);
	void setUser(std::string user);
	void setFd(int fd);
	void setNick(std::string nick);
	void setPass(std::string pass);
	void setBuffer(std::string buf);
	void clearBuffer(void);
	std::string getUser(void) const;
	std::string getNick(void) const;
	std::string getPass(void) const;
	int getFd(void) const;
	std::string getBuffer(void) const;
	bool isAuth();
	void send(std::string msg);
	~Client();

private:
	std::string _user;
	std::string _nick;
	std::string _pass;
	std::string _buffer;
	int _fd;
};
