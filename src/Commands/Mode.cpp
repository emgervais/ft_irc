#include "Command.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../util/util.hpp"
#include "NumericReplies.hpp"
#include <iostream>

static std::string  getChangeModes(std::string modes[2])
{
    std::string::iterator it = modes[0].begin();
    char sign = *it;
    std::string changeModes = "";
    changeModes += sign;
    
    ++it;
    while (it != modes[0].end())
    {
        if (*it == '+' || *it == '-')
        {
            if (*it != sign)
            {
                sign = *it;
                changeModes += sign;
            }
        }
        else
            changeModes += *it;
        ++it;
    }
    changeModes += modes[1];
    changeModes.insert(0, ":");
    return (changeModes);
}

void    Command::cmdMode()
{
    if (_params.size() == 0)
    {
        _client.addReply(ERR_NEEDMOREPARAMS(_client.getNick(), _cmd));
        return;
    }
    Channel *channel = _server.getChannel(_params[0]);
    if (_params[0][0] != '#')
    {
        if (!_server.isNicknameTaken(_params[0]))
            _client.addReply(ERR_NOSUCHNICK(_client.getNick(), _params[0]));
        else if (toUpper(_params[0]) == toUpper(_client.getNick()))
        {
            if (_params.size() == 1)
                _client.addReply(RPL_UMODEIS(_client.getNick(), _client.getMode()));
            else
                _client.setMode(_params[1]);
        }
        else if (_params[0][0] != '#')
            _client.addReply(ERR_USERSDONTMATCH(_client.getNick()));
    }
    else if (!channel)
        _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[0]));
    else if (_params.size() == 1)
    {
        _client.addReply(RPL_CHANNELMODEIS(_client.getNick(), channel->getName(), channel->getChanModes()));
        _client.addReply(RPL_CREATIONTIME(_client.getNick(), channel->getName(), channel->getCreationTime()));
    }
    else
    {
        if (channel->isMode("o", _client.getNick()) == false)
            _client.addReply(ERR_CHANOPRIVSNEEDED(_client.getNick(), channel->getName()));
        else
            cmdModeLoop(channel);
    }
}

void    Command::cmdModeLoop(Channel *channel)
{
    std::string::iterator it = _params[1].begin();
    std::string mode = _params[1];
    std::vector<std::string>::iterator params;
    std::string param = "";
    char sign = '+';
    std::string modeChanges[2] = {"", ""};
    bool    modeChanged = false;

    if (_params.size() > 2)
        params = _params.begin() + 2;
    while (it != _params[1].end())
    {
        if (*it == '+' || *it == '-')
        {
            sign = *it;
            ++it;
            continue;
        }
        if (*it == 'o'|| *it == 'k' || *it == 'l')
        {
            if (params == _params.end() || (sign == '-' && *it == 'l'))
                param = "";
            else
                param = *params++;
        }
        if (*it == 'o')
            modeChanged = cmdModeO(channel, sign, param);
        else if (*it == 'k')
            modeChanged = cmdModeK(channel, sign, param);
        else if (*it == 'l')
            modeChanged = cmdModeL(channel, sign, param);
        else
            modeChanged = cmdModeOther(channel, sign, *it);
        if (modeChanged)
        {
            modeChanges[0] += std::string(1, sign) + std::string(1, *it);
            if (!param.empty())
                modeChanges[1] += " " + param;
        }
        ++it;
    }
    if (!modeChanges[0].empty())
    {
        std::string changeModes = RPL_MODE(_client.getPrefix(), channel->getName(), getChangeModes(modeChanges));
        _client.addReply(changeModes);
        channel->sendMessage(changeModes, _client.getNick());
    }
}

bool    Command::cmdModeO(Channel *channel, char sign, const std::string& param)
{
    if (sign == '-')
    {
        if (channel->isMode("o", param))
        {
            channel->removeMode("o", param);
            return true;
        }
    }
    else
    {
        if (param.empty())
            _client.addReply(ERR_SPECIFYOP(_client.getNick()));
        else if (!_server.isNicknameTaken(param))
            _client.addReply(ERR_NOSUCHNICK(_client.getNick(), param));
        else
        {
            if (!channel->isMode("o", param))
            {
                if (channel->isClientOnChannel(param))
                {
                    channel->addMode("o", param);
                    return true;
                }
            }
        }
    }
    return false;
}

bool    Command::cmdModeK(Channel *channel, char sign, const std::string& param)
{
    if (param.empty())
        _client.addReply(ERR_SPECIFYKEY(_client.getNick()));
    else
    {
        if (sign == '-')
        {
            if (channel->isMode("k", param))
            {
                channel->removeMode("k");
                return true;
            }
        }
        else
        {
            if (!channel->isMode("k"))
            {
                bool validKey = param.find(' ') == std::string::npos;
                if (validKey)
                {
                    channel->addMode("k", param);
                    return true;
                }
                else
                    _client.addReply(ERR_INVALIDKEY(_client.getNick(), channel->getName(), param));
            }
            else
                _client.addReply(ERR_INVALIDKEY(_client.getNick(), channel->getName(), param));
        }
    }
    return false;
}

bool    Command::cmdModeL(Channel *channel, char sign, const std::string& param)
{

    if (sign == '-' && channel->isMode("l"))
    {
        channel->removeMode("l");
        return true;
    }
    else
    {
        bool validLimit = param.find_first_not_of("0123456789") == std::string::npos;
        if (validLimit)
        {
            channel->removeMode("l");
            channel->addMode("l", param);
            return true;
        }
        else
            _client.addReply(ERR_INVALIDLIMIT(_client.getNick(), channel->getName(), param));
    }
    return false;
}

bool    Command::cmdModeOther(Channel *channel, char sign, char mode)
{
    static const std::string OTHERMODES = "inst";
    if (OTHERMODES.find(mode) == std::string::npos)
    {
        _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), mode));
        return false;
    }
    if (sign == '-')
    {
        if (channel->isMode(std::string(1, mode)))
        {
            channel->removeMode(std::string(1, mode));
            return true;
        }
    }
    else
    {
        if (!channel->isMode(std::string(1, mode)))
        {
            channel->addMode(std::string(1, mode), _client.getNick());
            return true;
        }
    }
    return false;
}