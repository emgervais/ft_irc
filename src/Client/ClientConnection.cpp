#include "Client.hpp"

void    Client::joinChannel(const std::string& channel, const std::string& key)
{
    Channel* chan = _server.getChannel(channel);
    if (chan)
    {
        if (chan->isInviteOnly() && !chan->isInvited(*this))
        {
            addReply(ERR_INVITEONLYCHAN(_nick, channel));
            return;
        }
        if (chan->getKey() != key)
        {
            addReply(ERR_BADCHANNELKEY(_nick, channel));
            return;
        }
        chan->addClient(this);
        addReply(RPL_JOIN(_nick, _user, _hostname, channel));
        if (chan->getTopic() != "")
        {
            addReply(RPL_TOPIC(_nick, channel, chan->getTopic()));
            addReply(RPL_TOPICWHOTIME(_nick, channel, _user, _hostname, "Time not implemented yet"));
        }
    }
    else
    {
        _server.createChannel(channel, *this, key);
        chan = _server.getChannel(channel);
        chan->addClient(this);
        addReply(RPL_JOIN(_nick, _user, _hostname, channel));
    }
    addReply(RPL_NAMREPLY(_nick, channel, chan->getNamesReply()));
    addReply(RPL_ENDOFNAMES(_nick, channel));
}

void    Client::partChannel(const std::string& channel, const std::string& reason)
{
    Channel* chan = _server.getChannel(channel);
    if (chan)
    {
        if (_channels.find(channel) != _channels.end())
        {
            chan->removeClient(this, reason);
            addReply(RPL_PART(_nick, _user, _hostname, channel, reason));
        }
        else
            addReply(ERR_NOTONCHANNEL(_nick, channel));
    }
    else
        addReply(ERR_NOSUCHCHANNEL(_nick, channel));
}

void    Client::sendMessage(std::vector<std::string> targets, const std::string& message)
{
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        if (std::string(*it).find_first_of("#") == 0)
        {
            Channel* chan = _server.getChannel(*it);
            if (chan)
            {
                if (_channels.find(*it) != _channels.end())
                    chan->sendMessage(message);
                else
                    addReply(ERR_NOTONCHANNEL(_nick, *it));
            }
            else
                addReply(ERR_NOSUCHCHANNEL(_nick, *it));
        }
        else
        {
            if (_server.isNicknameTaken(*it) && *it != _nick)
                addReply(":" + _nick + "!" + _user + "@" + _hostname + " PRIVMSG " + *it + " :" + message + CRLF);
            else
                addReply(ERR_NOSUCHNICK(_nick, *it));
        }
    }
}