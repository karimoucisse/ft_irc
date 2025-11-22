#include "Main.hpp"

bool isError(int fd, Channel *channel ,std::string password)
{
	if(!channel)
		return false; // ERR_NOSUCHCHANNEL 403
	if(channel->getPassword() != password)
		return false; // ERR_PASSWDMISMATCH 464
	if(channel->getInviteOnly() && !channel->getInvitedClient(fd))
		return false; // ERR_INVITEONLYCHAN 473
	// if(channel limit)
	// 	return false; ERR_CHANNELISFULL (471)
	// if(user limit)w
	// 	 ERR_TOOMANYCHANNELS (405)
}
void join(Client *client, Channel *channel ,std::string password)
{
	if(isError(client->getFd(), channel, password))
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
