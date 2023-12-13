#include "Server.hpp"

// -- misc ----
std::string Server::getPass() const
{
    return _pass;
}

bool Server::isNicknameTaken(const std::string& nickname)
{
    std::map<int, Client*>::const_iterator it;
    int i = 0;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (!it->second)
            std::cout << "Client " << i << " is NULL" << std::endl;
        else if (toUpper(it->second->getNick()) == toUpper(nickname))
            return true;
        i++;
    }
    return false;
}

Channel* Server::getChannel(const std::string& name) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
            return it->second;
    }
    return NULL;
}

Client* Server::getClient(const std::string& nick) const
{
    std::map<int, Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNick() == nick)
            return it->second;
    }
    return NULL;
}

void Server::createChannel(const std::string& name, const Client& client, const std::string& key)
{
    _channels[name] = new Channel(name, client, *this, key);
}

void Server::removeChannel(const std::string& name)
{
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
        {
            delete it->second;
            _channels.erase(it);
            return;
        }
    }
}

std::string Server::getChannelReply(const std::string& name, const std::string& clientNick) const
{
    std::map<std::string, Channel*>::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->first == name)
            return (RPL_LIST(clientNick, it->second->getName(), it->second->getUsersCount(), it->second->getTopic()));
    }
    return "";
}

std::vector<std::string> Server::getChannelsReply(const std::string& clientNick) const
{
    std::vector<std::string> channelsList;
    std::map<std::string, Channel*>::const_iterator it;

    for (it = _channels.begin(); it != _channels.end(); ++it)
        channelsList.push_back(RPL_LIST(clientNick, it->second->getName(), it->second->getUsersCount(), it->second->getTopic()));
    return channelsList;
}

void    Server::writeToClients(std::vector<int> sockets, const std::string& msg)
{
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (std::find(sockets.begin(), sockets.end(), it->first) == sockets.end())
            it->second->addReply(msg);
}