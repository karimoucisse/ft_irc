#include "Irc.hpp"

bool checkPort(std::string strPort)
{
	if (strPort.find_first_not_of("0123456789") != std::string::npos)
		return (std::cerr << RED << "Error: port must only contain numbers" << RESET << std::endl, false);
	int port = std::atoi(strPort.c_str());
	if (port < 1024 || port > 65535)
		return (std::cerr << RED << "Error: Port must be between 1024-65535" << RESET << std::endl, false);
	return true;
}
std::vector<std::string> splitCmdLine(std::string str)
{
	std::vector<std::string> cmds;
	std::istringstream stm(str);
	std::string line;
	while (std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		cmds.push_back(line);
	}
	return cmds;
}
std::vector<std::string> splitCmd(std::string input)
{
	std::vector<std::string> out;
	std::string commandLine = input;

	std::istringstream ss(commandLine);
	std::string token;

	if (!(ss >> token))
		return out;

	for (char &c : token)
		c = std::tolower(static_cast<unsigned char>(c));

	out.push_back(token);

	while (ss >> token)
		out.push_back(token);

	return out;
}
