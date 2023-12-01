#include "Client.hpp"
#include "Server.hpp"

Client::Client(int socket, Server &server)
    : _socket(socket), _server(server), _registered(false)
{

}

Client::Client(int socket, std::string nick, std::string user, Server &server)
    : _socket(socket), _nick(nick), _user(user), _server(server), _registered(false)
{
    
}

Client::Client(Client const& rhs)
    : _socket(rhs._socket), _nick(rhs._nick), _user(rhs._user), _server(rhs._server), _registered(false)
{
    
}

Client& Client::operator=(const Client& rhs)
{
    if (this != &rhs)
    {
        _socket = rhs._socket;
        _nick = rhs._nick;
        _user = rhs._user;
        _hostname = rhs._hostname;
        _registered = rhs._registered;
        _server = rhs._server;
    }
    return *this;
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

int             Client::getSocket() const
{
    return (_socket);
}

Server      Client::getServer() const
{
    return (_server);
}

bool            Client::isRegistered() const
{
    return (_registered);
}

void            Client::setNick(std::string nick)
{
    _nick = nick;
}

void            Client::setUser(std::string user)
{
    _user = user;
}

void            Client::setRegistered(bool registered)
{
    _registered = registered;
}
