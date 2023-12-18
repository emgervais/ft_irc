#include "Client.hpp"
#include "../Server/Server.hpp"
#include "../Channel/Channel.hpp"
#include "NumericReplies.hpp"

void    Client::joinChannel(const std::string& channel, const std::string& key)
{
    Channel* chan = _server.getChannel(channel);
    
    if (chan)
    {
        if (chan->isClientOnChannel(*this))
        {
            addReply(ERR_USERONCHANNEL(_nick, _nick, channel));
            return;
        }
        if (chan->isMode("i") && !chan->isMode("i", _nick))
        {
            addReply(ERR_INVITEONLYCHAN(_nick, channel));
            return;
        }
        if (chan->isMode("k") && !chan->isMode("k", key))
        {
            addReply(ERR_BADCHANNELKEY(_nick, channel));
            return;
        }
        if (chan->isMode("l") && chan->canJoin() == false)
        {
            addReply(ERR_CHANNELISFULL(_nick, channel));
            return;
        }
        chan->addClient(this);
    }
    else
    {
        _server.createChannel(channel, *this, key);
        chan = _server.getChannel(channel);
    }
    addReply(RPL_JOIN(getPrefix(), channel));
    if (chan->getTopic() != "")
    {
        addReply(RPL_TOPIC(_nick, channel, chan->getTopic()));
        addReply(RPL_TOPICWHOTIME(_nick, channel, _user, chan->getTopic(), chan->getTopicTime()));
    }
    _channels[channel] = chan;
    addReply(RPL_NAMREPLY(_nick, channel, chan->getNamesReply(*this)));
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
            addReply(RPL_PART(getPrefix(), channel, reason));
        }
        else
            addReply(ERR_NOTONCHANNEL(_nick, channel));
    }
    else
        addReply(ERR_NOSUCHCHANNEL(_nick, channel));
}

void    Client::partAllChannels()
{
    if (_channels.size() == 0)
        return;
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
        partChannel(it->first, "Disconnected from server");
}

void    Client::sendMessage(std::vector<std::string> targets, const std::string& message)
{
    for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        if (std::string(*it).find_first_of("#") == 0 || std::string(*it).find_first_of("@#") == 0)
        {
            std::string channelName = std::string(*it).find_first_of("@") == 0 ? std::string(*it).substr(1) : *it;
            Channel* chan = _server.getChannel(channelName);
            if (!chan)
                addReply(ERR_NOSUCHCHANNEL(_nick, *it));
            else if (chan->isClientOnChannel(*this) || chan->isMode("n") == false)
            {
                if (std::string(*it).find_first_of("@") == 0)
                    chan->sendMessageToOps(RPL_PRIVMSG(getPrefix(), *it, message), _nick);
                else
                    chan->sendMessage(RPL_PRIVMSG(getPrefix(), *it, message), _nick);
            }
            else
                addReply(ERR_CANNOTSENDTOCHAN(_nick, *it));
        }
        else
        {
            if (_server.isNicknameTaken(*it) && *it != _nick)
            {
                Client* target = _server.getClient(*it);
                target->addReply(RPL_PRIVMSG(getPrefix(), *it, message));
            }
            else
                addReply(ERR_NOSUCHNICK(_nick, *it));
        }
    }
}