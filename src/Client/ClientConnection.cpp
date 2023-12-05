#include "Client.hpp"

bool    Client::isOnChannel(const std::string& channel) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == channel)
            return true;
    }
    return false;
}

bool    Client::isOnChannel(const Channel& channel) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second == &channel)
            return true;
    }
    return false;
}

void    Client::partChannel(const std::string& channel)
{
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == channel)
        {
            it->second->removeClient(this);
            _channels.erase(it);
            return;
        }
    }
}

void    Client::partChannel(Channel *channel)
{
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second == channel)
        {
            it->second->removeClient(this);
            _channels.erase(it);
            return;
        }
    }
}

void    Client::joinChannel(const std::string& channel)
{
    _channels[channel] = new Channel(channel, *this, _server);
}

void    Client::joinChannel(Channel *channel)
{
    _channels[channel->getName()] = channel;
}

void    Client::sendMessage(const std::string& msg, const std::string& channel)
{
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == channel)
        {
            it->second->sendMessage(msg);
            return;
        }
    }
}