#include "Irc.hpp"

void Server::cmd_pass(std::vector<std::string> cmd, Client *client)
{
	std::cout << "yesy yes!" << std::endl;
	// if(cmd.size() < 2)

	// if (client->isAuth())
	// 	return client->send(irc_err_alreadyregistred(client->getNick()));
	// ;
	// std::string password = event->message->to<std::string>();
	// if (password.empty())
	// 	return client->send(irc_err_needmoreparams(client->getNick(), password)); // ERR_NEEDMOREPARAMS
	// if (server->checkPassword(password))
	// 	client->setIsPassCorrect(true);
}

// :irc.example.com 461 user42 USER :Not enough parameters
// Décomposition :

// irc.example.com → le serveur qui envoie le message

// 461 → code numérique (error code)

// user42 → l’utilisateur cible de l’erreur

// USER → commande qui a posé problème

// Not enough parameters → message associé
