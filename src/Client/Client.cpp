#include "Client.hpp"
#include "Server.hpp"

Client::Client(int socket, Server &server)
    : _socket(socket), _nick("NICK"), _user("USER"), _realname("RealName"), _hostname("HostName"), _server(server), _registered(false)
{

}

// // Probably not useful
// Client::Client(Client const& rhs)
//     : _socket(rhs._socket), _nick(rhs._nick), _user(rhs._user), _server(rhs._server), _registrationStep(rhs._registrationStep)
// {
    
// }

// // Probably not useful
// Client& Client::operator=(const Client& rhs)
// {
//     if (this != &rhs)
//     {
//         _socket = rhs._socket;
//         _nick = rhs._nick;
//         _user = rhs._user;
//         _hostname = rhs._hostname;
//         _realname = rhs._realname;
//         _registrationStep = rhs._registrationStep;
//         _server = rhs._server;
//     }
//     return *this;
// }


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