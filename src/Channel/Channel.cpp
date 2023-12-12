#include "Channel.hpp"

// t = topic takes no parameter
// k = key takes parameter if +, no parameter if -
// l = limit takes parameter if +, no parameter if -
// i = invite only takes no parameter
// o = operator always takes parameter

Channel::Channel(const std::string& name, const Client& client, const Server& server, const std::string& key)
    : _name(name), _topic(""), _server(const_cast<Server&>(server)), _creationTime(getTimeOfDay())
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
    {
        if (*it == client)
            return;
    }
    _clients.push_back(client);
    sendMessage(RPL_JOIN(client->getNick(), client->getUser(), client->getHostname(), _name), client->getNick());
}

void    Channel::removeClient(Client *client, const std::string& reason)
{
    std::vector<Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == client)
        {
            sendMessage(RPL_PART(client->getNick(), client->getUser(), client->getHostname(), _name, reason), client->getNick());
            removeAllModes(*client);
            _clients.erase(it);
            if (_clients.size() == 0)
                _server.removeChannel(_name);
            return;
        }
    }
}

void    Channel::setTopic(const std::string& topic)
{
    _topic = topic;
    sendMessage(RPL_TOPIC(_clients[0]->getNick(), _name, _topic), _clients[0]->getNick());
}

std::string     Channel::getName() const
{
    return (_name);
}

std::string     Channel::getTopic() const
{
    return (_topic);
}

int             Channel::getUsersCount() const
{
    return (_clients.size());
}

void    Channel::sendMessage(const std::string& msg, const std::string& sender)
{
    std::vector<Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNick() != sender)
        {
            (*it)->addReply(msg);
        }
    }
}

bool    Channel::isMode(const std::string& mode) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode && it->second.size() > 0)
            return true;
    }
    return false;
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
                if (*it2 == param)
                    return true;
        }
    }
    return false;
}

void    Channel::addMode(const std::string& mode, const std::string& param)
{
    if (isMode(mode, param))
        return;
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            it->second.push_back(param);
            return;
        }
    }
    std::vector<std::string> v;
    v.push_back(param);
    _modes[mode] = v;
}

void    Channel::removeMode(const std::string& mode)
{
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = _modes.begin(); it != _modes.end(); ++it)
    {
        if (it->first == mode)
        {
            if (mode != "o")
                _modes.erase(it);
            return;
        }
    }
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

bool    Channel::isClientOnChannel(const Client& client) const
{
    std::vector<Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it == &client)
            return true;
    }
    return false;
}

bool    Channel::isClientOnChannel(const std::string& nick) const
{
    std::vector<Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNick() == nick)
            return true;
    }
    return false;
}

std::string     Channel::getModeString() const
{
    std::string mode = "+";
    std::string addLimit = "";
    std::map<std::string, std::vector<std::string> >::const_iterator it = _modes.begin();
    
    for (; it != _modes.end(); ++it)
    {
        if (it->first == "o")
            continue;
        else if (it->first == "l")
        {
            mode += it->first;
            addLimit = it->second[0];
        }
    }
    if (addLimit != "")
        mode += " :" + addLimit;
    return mode;
}

std::string     Channel::getNamesReply() const
{
    std::string reply;
    std::vector<Client*>::const_iterator it;
    
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (isMode("o", (*it)->getNick()))
            reply += "@";
        reply += (*it)->getNick() + " ";
    }
    return reply;
}

void    Channel::removeAllModes(const Client& client)
{
    char    mode[3] = {'o', 'i', '\0'};

    for (int i = 0; mode[i]; ++i)
        removeMode(std::string(1, mode[i]), client.getNick());
}