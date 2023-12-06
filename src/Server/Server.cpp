#include "Server.hpp"
#include "NumericReplies.hpp"

// -- init --
Server& Server::getInstance(int port, std::string const& password)
{
    static Server instance(port, password);
    if (port != instance._port || password != instance._pass)
        throw std::runtime_error("Only one server instance can exist.");
    return instance;
}

Server::Server(int port, std::string const& password)
    : _port(port), _pass(password), _maxClients(MAX_CLIENTS)
{
    try
    {
        initSocket();
        initKqueue();
        Command::initCmdHandler();
    }

    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void Server::initSocket()
{
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1)
        throw std::runtime_error("Error: socket creation failed");

    int opt = 1; 
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw std::runtime_error("Error: socket setsockopt failed");
    
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);

    if (bind(_socket, (sockaddr *)&_addr, sizeof(_addr)) == -1)
        throw std::runtime_error("Error: socket bind failed");
    
    if (listen(_socket, MAX_CLIENTS) == -1)
        throw std::runtime_error("Error: socket listen failed");
}

void Server::initKqueue()
{
    _kqueue = kqueue();
    if (_kqueue == -1)
        throw std::runtime_error("Error: kqueue creation failed");
    EV_SET(_events, _socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("Error: kqueue event creation failed");
}

// -- end ----
Server::~Server()
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        closeClient(it->first, false);
    }
    close(_socket);
}

void Server::closeClient(int socket, bool erase)
{
    CLOSE_CONNECTION_MSG(socket);
    delete _clients[socket];
    close(socket);
    if (erase)
        _clients.erase(socket);
}

// -- misc ----
std::string Server::getPass() const
{
    return (_pass);
}

std::string toUpper(const std::string& str)
{
    std::string upper;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        upper += toupper(*it);
    }
    return (upper);
}

bool Server::isNicknameTaken(const std::string& nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (toUpper(it->second->getNick()) == toUpper(nickname))
            return true;
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

