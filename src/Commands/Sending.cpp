#include "Command.hpp"
#include "util.hpp"

void    Command::cmdPrivMsg()
{
	if (_params.size() < 2)
	{
		_client.addReply(ERR_NOTEXTTOSEND(_client.getNick()));
		return;
	}
	std::vector<std::string> targets = splitString(_params[0], ',');
	std::vector<std::string> msgParams(_params.begin() + 1, _params.end());
	std::string const& msg = contcatParams(msgParams);
	_client.sendMessage(targets, msg);
}
