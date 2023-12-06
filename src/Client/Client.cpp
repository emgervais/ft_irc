#include "Client.hpp"
#include "Server.hpp"

Client::Client(int socket, Server &server)
    : _socket(socket), _nick("NICK"), _user("USER"), _realname("RealName"), _hostname("HostName"), _server(server), _registered(false), _sendQueue()
{

}

Client::~Client()
{
    
}

std::string     Client::getNick() const
{
    return (_nick);
}

std::string     Client::getUser() const
{
    return (_user);
}

std::string     Client::getRealname() const
{
    return (_realname);
}

std::string     Client::getHostname() const
{
    return (_hostname);
}

int             Client::getSocket() const
{
    return (_socket);
}

bool            Client::isRegistered() const
{
    return (_registered);
}

void            Client::addReply(std::string const& reply)
{
    _sendQueue.push(reply);
}

void            Client::removeReply()
{
    _sendQueue.pop();
}

std::string     Client::getReply() const
{
    if (_sendQueue.empty())
    {
        return ("");
    }
    return _sendQueue.front();
}