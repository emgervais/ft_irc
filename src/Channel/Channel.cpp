#include "Channel.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include "../util/util.hpp"
#include "NumericReplies.hpp"
#include <sstream>


Channel::Channel(const std::string& name, const Client& client, const Server& server, const std::string& key)
    : _name(name), _topic(""), _server(const_cast<Server&>(server)),
    _creationTime(getUnixTime())
{
    _clients.push_back(const_cast<Client*>(&client));
    addMode("o", client.getNick());
    if (key != "")
        addMode("k", key);
}

Channel::~Channel()
{
}

void    Channel::addClient(Client *client)
{
    std::vector<Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (*it == client)
            return;
    _clients.push_back(client);
    sendMessage(RPL_JOIN(client->getPrefix(), _name), client->getNick());
}

void    Channel::removeClient(Client *client, const std::string& reason, Client *kicker)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            if (kicker)
            {
                sendMessage(RPL_KICK(kicker->getPrefix(), _name, client->getNick(), reason), kicker->getNick());
                sendMessage(RPL_KICK(kicker->getPrefix(), _name, client->getNick(), reason), client->getNick());
            }
            else
                sendMessage(RPL_PART(client->getPrefix(), _name, reason), client->getNick());
            removeAllModes(*client);
            _clients.erase(it);
            if (_clients.size() == 0)
                _server.removeChannel(_name);
            return;
        }
    }
}

bool    Channel::isClientOnChannel(const Client& client) const
{
    std::vector<Client*>::const_iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (*it == &client)
            return true;
    return false;
}

bool    Channel::isClientOnChannel(const std::string& nick) const
{
    std::vector<Client*>::const_iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if (toUpper((*it)->getNick()) == toUpper(nick))
            return true;
    return false;
}

bool    Channel::canJoin() const
{
    if (isMode("l"))
    {
        std::stringstream ss(_modes.find("l")->second[0]);
        int limit;
        
        ss >> std::noskipws >> limit;
        if (!(ss.eof() && !ss.fail()) || _clients.size() >= static_cast<size_t>(limit))
            return false;
    }
    return true;
}

std::string     Channel::getName() const
{
    return (_name);
}

std::string     Channel::getTopic() const
{
    return (_topic);
}

std::string     Channel::getTopicTime() const
{
    return (_topicTime);
}

void    Channel::setTopic(const std::string& topic)
{
    _topic = topic;
    _topicTime = getUnixTime();
    sendMessage(RPL_TOPIC(_clients[0]->getNick(), _name, _topic), _clients[0]->getNick());
}

std::string     Channel::getNamesReply(const Client &client) const
{
    std::string reply;
    std::vector<Client*>::const_iterator it;
    
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->isInvisible() && !isClientOnChannel(client))
            continue;
        if (isMode("o", (*it)->getNick()))
            reply += "@";
        reply += (*it)->getNick() + " ";
    }
    return reply;
}

std::string     Channel::getNamesReply(const std::string& clientNick) const
{
    std::string reply;
    std::vector<Client*>::const_iterator it;
    
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->isInvisible() && !isClientOnChannel(clientNick))
            continue;
        if (isMode("o", (*it)->getNick()))
            reply += "@";
        reply += (*it)->getNick() + " ";
    }
    return reply;
}

std::string     Channel::getCreationTime() const
{
    return _creationTime;
}

void    Channel::sendMessage(const std::string& msg, const std::string& sender)
{
    std::vector<Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if ((*it)->getNick() != sender)
            (*it)->addReply(msg);
}

void    Channel::sendMessageToOps(const std::string& msg, const std::string& sender)
{
    std::vector<Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
        if ((*it)->getNick() != sender && isMode("o", (*it)->getNick()))
            (*it)->addReply(msg);
}
