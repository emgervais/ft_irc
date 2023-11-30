#include "Client.hpp"


Client::Client(int socket, Server &server) : _socket(socket), _server(server)
{

}

Client::Client(int socket, std::string nick, std::string user, Server &server) : _socket(socket), _nick(nick), _user(user), _server(server)
{
    
}

Client::~Client()
{
    
}

std::string     Client::getNick()
{
    return (_nick);
}

std::string     Client::getUser()
{
    return (_user);
}

int             Client::getSocket()
{
    return (_socket);
}

Server      Client::getServer()
{
    return (_server);
}

bool            Client::isRegistered()
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
