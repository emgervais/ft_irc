#include "Command.hpp"

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
    if (!modes[1].empty())
    {
        ssize_t pos = modes[1].find_last_of(' ');
        modes[1].insert(pos, ":");
        changeModes += modes[1];
    }
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
    if (!channel)
        _client.addReply(ERR_NOSUCHCHANNEL(_client.getNick(), _params[0]));
    else if (_params.size() == 1)
        _client.addReply(RPL_CHANNELMODEIS(_client.getNick(), channel->getName(), channel->getChanModes()));
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
    bool modeChange = false;

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
            if (params == _params.end() || (sign == '-' && *it != 'o'))
                param = "";
            else
                param = *params++;
        }
        else if (*it == 'i')
            modeChange = cmdModeI(channel, sign);
        else if (*it == 'o')
            modeChange = cmdModeO(channel, sign, param);
        else if (*it == 'k')
            modeChange = cmdModeK(channel, sign, param);
        else if (*it == 'l')
            modeChange = cmdModeL(channel, sign, param);
        else if (*it == 't')
            modeChange = cmdModeT(channel, sign);
        else
        {
            modeChange = false;
            _client.addReply(ERR_UNKNOWNMODE(_client.getNick(), *it));
        }
        if (modeChange)
        {
            modeChanges[0] += sign + *it;
            if (!param.empty())
                modeChanges[0] += " " + param;
        }
        ++it;
    }
    if (!modeChanges[0].empty())
        _client.addReply(RPL_MODE(_client.getNick(), _client.getUser(), _client.getHostname(), channel->getName(), getChangeModes(modeChanges)));
}

bool    Command::cmdModeO(Channel *channel, char sign, const std::string& param)
{
    if (param.empty())
        _client.addReply(ERR_SPECIFYOP(_client.getNick()));
    else
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

bool    Command::cmdModeI(Channel *channel, char sign)
{
    if (sign == '-')
    {
        if (channel->isMode("i"))
        {
            channel->removeMode("i");
            return true;
        }
    }
    else
    {
        if (!channel->isMode("i"))
        {
            channel->addMode("i", _client.getNick());
            return true;
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
                if (param.find(' ') == std::string::npos)
                {
                    channel->addMode("k", param);
                    return true;
                }
                else
                    _client.addReply(ERR_INVALIDKEY(_client.getNick(), channel->getName(), param));
            }
        }
    }
    return false;
}

bool    Command::cmdModeL(Channel *channel, char sign, const std::string& param)
{
    if (param.empty())
        _client.addReply(ERR_SPECIFYLIMIT(_client.getNick()));
    else
    {
        if (sign == '-' && channel->isMode("l"))
        {
            channel->removeMode("l");
            return true;
        }
        else
        {
            if (param.find_first_not_of("0123456789") == std::string::npos)
            {
                channel->addMode("l", param);
                return true;
            }
            else
                _client.addReply(ERR_INVALIDLIMIT(_client.getNick(), channel->getName(), param));
        }
    }
    return false;
}

bool    Command::cmdModeT(Channel *channel, char sign)
{
    if (sign == '-')
    {
        if (channel->isMode("t"))
        {
            channel->removeMode("t");
            return true;
        }
    }
    else
    {
        if (!channel->isMode("t"))
        {
            channel->addMode("t", _client.getNick());
            return true;
        }
    }
    return false;
}
