#include "Client.hpp"

bool    Client::isOnChannel(const std::string& channel) const
{
    if (_channels.find(channel) != _channels.end())
        return (true);
    return (false);
}

void    Client::joinChannel(const std::string& channel, const std::string& key)
{
    Channel* chan;
    if (_server.isChannelNameTaken(channel))
    {
        chan = _server.getChannel(channel);
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

void    Client::sendMessage(const std::string& msg, const std::string& channel)
{
    if (_channels.find(channel) != _channels.end())
    {
        _channels[channel]->sendMessage(msg);
    }
    else
    {
        addReply(ERR_NOSUCHCHANNEL(_nick, channel));
    }
}