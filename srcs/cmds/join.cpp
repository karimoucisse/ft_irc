#include "Main.hpp"



bool isError(std::string channelName, std::string password, int fd)
{
	if(channelName.empty())
		return false; // ERR_NEEDMOREPARAMS (461)
	if(channel->getPassword() != password)
		return false; // ERR_PASSWDMISMATCH 464
	if(channel->getInviteOnly() && !channel->getInvitedClient(fd))
		return false; // ERR_INVITEONLYCHAN 473
	// if(channel limit)
	// 	return false; ERR_CHANNELISFULL (471)
	// if(user limit)
	// 	 ERR_TOOMANYCHANNELS (405)
}

void split(std::string cmd, std::string &channelName, std::string &password)
{
	std::istringstream ss(cmd);
	std::vector<std::string> stringContainer;

	while(ss >> cmd);
	if(cmd.size() != 2 && cmd.size() != 3)
		return;
	channelName = cmd[1];
	if(cmd.size() == 3)
		password = cmd[2];
}
void Server::join(std::string cmd, int fd)
{
	std::string channelName;
	std::string password;
	split(cmd, channelName, password);
	if(isError(channelName, password, fd))
		return;
	channel->addClient(*client);
	channel->broadcastToAll("msg");
	// if channel is invite only, verify if user has been invited.
	// 	if user have been invited, then add user, and delete user
	// 	from invitedlist

	// if channel required a password, compare password
	// 	if passwords are the same, then add userw

	// brodcast to all client off the channel
}
