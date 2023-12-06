#include "Command.hpp"

void    Command::cmdQuit()
{
    if (_params.size() > 1)
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), contcatParams()));
    else
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), _params[0]));
}

void    Command::cmdJoin()
{
    if (_params.size() == 2 || _params.size() == 1)
    {
        std::map<std::string, std::string> channels;
        std::stringstream ssc(_params[0]);
        std::stringstream ssk(_params.size() == 2 ? _params[1] : "");
        std::string channel;
        std::string key;
        while (std::getline(ssc, channel, ','))
        {
            if (std::getline(ssk, key, ','))
                channels[channel] = key;
            else
                channels[channel] = "";
        }
        std::map<std::string, std::string>::iterator it;
        for (it = channels.begin(); it != channels.end(); ++it)
        {
            _client.joinChannel(it->first, it->second);
        }
    }
    else
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
}

// TODO:
// cmdPart
// cmdPing
// cmdPong