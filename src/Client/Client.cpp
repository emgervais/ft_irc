#include "Client.hpp"
#include "Server.hpp"

static std::string    getHostName(int socket)
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);
    char                hostname[NI_MAXHOST];

    if (getpeername(socket, (struct sockaddr *)&addr, &len) == -1)
        return ("");
    if (getnameinfo((struct sockaddr *)&addr, len, hostname, sizeof(hostname), NULL, 0, 0) == -1)
        return ("");
    return (std::string(hostname));
}

Client::Client(int socket, Server &server)
    : _socket(socket), _nick("NICK"), _user("USER"), _realname("RealName"), _hostname("HostName"), _server(server), _registrationStep(0)
{
    _reply = "";
    _newlyRegistered = false;
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

int             Client::getSocket() const
{
    return (_socket);
}

bool            Client::isRegistered() const
{
    return (_registrationStep == 3);
}

bool            Client::setNick(const std::string& nick)
{
    if (isNickValid(nick))
    {
        _nick = nick;
        _registrationStep++;
        return (true);
    }
    return (false);
}

bool            Client::setUser(std::vector<std::string> params)
{
    if (isUserValid(params))
    {
        _user = params[1];
        _realname = params[4];
        _hostname = getHostName(_socket);
        _registrationStep++;
        return (true);
    }
    return (false);
}

bool            Client::setRealname(std::string realname)
{
    _realname = realname;
    return (true);
}

bool            Client::setHostname(std::string hostname)
{
    _hostname = hostname;
    return (true);
}

bool            Client::checkPassword(std::string password)
{
    if (isPassValid(password))
    {
        _registrationStep++;
        return (true);
    }
    return (false);
}

std::string     Client::getReply(void)
{
    std::string tmp = _reply;

    if (_reply.empty())
        tmp = "No reply";
    _reply = "";
    return (tmp);
}

void        Client::setNewlyRegistered(bool newlyRegistered)
{
    _newlyRegistered = newlyRegistered;
}

bool        Client::isNewlyRegistered() const
{
    return (_newlyRegistered);
}