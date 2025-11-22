
#include "Main.hpp"
Client::Client(void) : _pass(""), _nick(""), _user(""), _fd(-1) {}
Client::Client(int fd) : _pass(""), _nick(""), _user(""), _fd(fd) {}
Client::Client(std::string p, std::string n, std::string u) : _pass(""), _nick(n), _user(u), _fd(-1) {}
Client::Client(const Client &client)
{
	*this = client;
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		this->_user = client.getUser();
		this->_nick = client.getNick();
		this->_pass = client.getPass();
		this->_fd = client.getFd();
	}
	return *this;
}
void Client::setUser(std::string user)
{
	_user = user;
}
void Client::setFd(int fd)
{
	_fd = fd;
}
void Client::setNick(std::string nick)
{
	_nick = nick;
}
void Client::setPass(std::string pass)
{
	_pass = pass;
}
std::string Client::getUser(void) const
{
	return _user;
}
std::string Client::getNick(void) const
{
	return _nick;
}
std::string Client::getPass(void) const
{
	return _pass;
}
int Client::getFd(void) const
{
	return _fd;
}
bool Client::isAuth()
{
	return (_fd != -1 && !_pass.empty() && !_nick.empty() && !_user.empty());
}
Client::~Client()
{
	// if(_fd != -1)
	// {
	// 	close(_fd);
	// 	std::cout << _nick  << " has been deleted !" << std::endl;
	// }
}
