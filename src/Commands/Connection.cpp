#include "Command.hpp"
#include "NumericReplies.hpp"
#include "../util/util.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include <sstream>

void Command::cmdQuit()
{
    if (_params.size() > 1)
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), contcatParams(_params)));
    else
        _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), _params[0]));
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
        size_t targets = 0;

        while (std::getline(ssc, channel, ',') && targets < MAX_TARGETS)
        {
            if (std::getline(ssk, key, ','))
                channels[channel] = key;
            else
                channels[channel] = "";
            if (channel[0] != '#')
                _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), channel));
            else
                targets++;
        }
        std::map<std::string, std::string>::iterator it;
        for (it = channels.begin(); it != channels.end(); ++it)
            _client.joinChannel(it->first, it->second);
    }
    else
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
}

void Command::cmdPart()
{
    std::vector<std::string> lastParams(_params.begin() + 1, _params.end());
    std::string reason = contcatParams(lastParams);
    size_t targets = 0;

    if (_params.size() > 0)
    {
        std::stringstream ss(_params[0]);
        std::string channel;
        while (std::getline(ss, channel, ',') && targets < MAX_TARGETS)
        {
            _client.partChannel(channel, reason);
            targets++;
        }
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
            _client.addReply(
                std::string(RPL_WELCOME(_client.getNick(), _client.getUser(), _client.getHostname()))
                + std::string(RPL_YOURHOST(_client.getNick()))
                + std::string(RPL_CREATED(_client.getNick()))
                + std::string(RPL_MYINFO(_client.getNick()))
                + std::string(RPL_ISUPPORT(_client.getNick()))
            );
            return;
        }
        else
        {
            _client.addReply(RPL_QUIT(_client.getUser(), _client.getHostname(), "[Incorrect ping reply for registration]"));
            _client.setClosing();
        }
    }
}