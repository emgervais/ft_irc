#include "Command.hpp"
#include "util.hpp"

void Command::cmdQuit()
{
    if (_params.size() > 1)
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), contcatParams(_params)));
    else
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), _params[0]));
    _client.partAllChannels();
    _client.setClosing();
}

void Command::cmdJoin()
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
            if (channel[0] != '#')
                _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), channel));
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

void Command::cmdPart()
{
    std::vector<std::string> lastParams(_params.begin() + 1, _params.end());
    std::string reason = contcatParams(lastParams);

    if (_params.size() > 0)
    {
        std::stringstream ss(_params[0]);
        std::string channel;
        while (std::getline(ss, channel, ','))
            _client.partChannel(channel, reason);
    }
}

void Command::cmdPing()
{
    if (_params.size() == 1)
        _client.addReply(RPL_PONG(_client.getNick(), SERVER_NAME, _params[0]));
    else if (_params.size() > 1)
        _client.addReply(RPL_PONG(_client.getNick(), _params[1], _params[0]));
    else
    {
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
        _client.addReply(RPL_PINGUSE(_client.getNick()));
    }
}

void Command::cmdPong()
{
    if (!_client.isRegistered() && _client.isWaitingForPong())
    {
        if (_params.size() != 0 && _client.getPing() == _params[0])
        {
            _client.setRegistered();
            _client.addReply(RPL_WELCOME(_client.getNick(), _client.getUser(), _client.getHostname()));
            _client.addReply(RPL_YOURHOST(_client.getNick()));
            _client.addReply(RPL_CREATED(_client.getNick()));
            _client.addReply(RPL_MYINFO(_client.getNick()));
            _client.addReply(RPL_ISUPPORT(_client.getNick()));
            return;
        }
        else
        {
            _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), "[Incorrect ping reply for registration]"));
            _client.setClosing();
        }
    }
}