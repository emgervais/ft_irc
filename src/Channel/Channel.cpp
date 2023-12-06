#include "Channel.hpp"

Channel::Channel(const std::string& name, const Client& client, const Server& server, const std::string& key)
    : _name(name), _topic(""), _key(key), _server(const_cast<Server&>(server))
{
    _clients.push_back(const_cast<Client*>(&client));
    _modes["o"].push_back(client.getNick());
}

Channel::~Channel()
{
}

void    Channel::addClient(Client *client)
{
    _clients.push_back(client);
}

void    Channel::removeClient(Client *client)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            _clients.erase(it);
            return;
        }
    }
}

void    Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}

std::string     Channel::getName() const
{
    return (_name);
}

std::string     Channel::getTopic() const
{
    return (_topic);
}

std::vector<Client*>    Channel::getClients() const
{
    return (_clients);
}

bool    Channel::isClientInChannel(const Client& client) const
{
    std::vector<Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == &client)
            return true;
    }
    return false;
}

bool    Channel::isOp(const Client& client) const
{
    return (isMode("o", client.getNick()));
}

bool    Channel::isInvited(const Client& client) const
{
    return (isMode("i", client.getNick()));
}

void    Channel::sendMessage(const std::string& msg)
{
    std::vector<int>    sockets;
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        sockets.push_back((*it)->getSocket());
    }
    _server.writeToClients(sockets, msg);
}

bool    Channel::isMode(const std::string& mode, const std::string& param) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            std::vector<std::string>::const_iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                if (*it2 == param)
                    return true;
            }
        }
    }
    return false;
}

void    Channel::addMode(const std::string& mode, const std::string& param)
{
    if (isMode(mode, param))
        return;
    _modes[mode].push_back(param);
}

void    Channel::removeMode(const std::string& mode, const std::string& param)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            std::vector<std::string>::iterator it2;
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                if (*it2 == param)
                {
                    it->second.erase(it2);
                    return;
                }
            }
        }
    }
}

void    Channel::setKey(const std::string& key)
{
    _key = key;
}

std::string     Channel::getKey() const
{
    return (_key);
}

bool    Channel::isInviteOnly()
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == "i")
            return true;
    }
    return false;
}

std::string     Channel::getModeString() const
{
    std::string mode;
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        mode += it->first;
    }
    return mode;
}

// RPL_NAMREPLY (353) 
//   "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
// Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.

// <symbol> notes the status of the channel. It can be one of the following:

// ("=", 0x3D) - Public channel.
// ("@", 0x40) - Secret channel (secret channel mode "+s").
// ("*", 0x2A) - Private channel (was "+p", no longer widely used today).
// <nick> is the nickname of a client joined to that channel, and <prefix> is the highest channel membership prefix that client has in the channel, if they have one. The last parameter of this numeric is a list of [prefix]<nick> pairs, delimited by a SPACE character (' ', 0x20).
std::string     Channel::getNamesReply() const
{
    std::string reply;
    std::vector<Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (isOp(**it))
            reply += "@";
        else
            reply += "=";
        reply += (*it)->getNick() + " ";
    }
    return reply;
}