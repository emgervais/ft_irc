#include "Command.hpp"

void    Command::cmdPrivMsg()
{
	if (_params.size() < 2)
	{
		_client.addReply(ERR_NOTEXTTOSEND(_client.getNick()));
		return;
	}
	std::vector<std::string> targets = splitString(_params[0], ",");
	std::vector<std::string> msgParts(_params.begin() + 1, _params.end());
	std::string msg = contcatParams(msgParts);
	_server.censor(msg);
	_client.sendMessage(targets, msg);
}
