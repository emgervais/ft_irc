#include "Channel.hpp"



Channel::Channel(const std::string& name, const Client& client, const Server& server)
    : _name(name), _server(const_cast<Server&>(server))
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
    _server.sendToClients(msg, sockets);
}