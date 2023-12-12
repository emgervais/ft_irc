#include <signal.h>
#include <thread>
#include "Server.hpp"
#include "NumericReplies.hpp"

// -- singleton ----
Server& Server::getInstance(int port, std::string const& password)
{
    static Server instance(port, password);
    if (port != instance._port || password != instance._pass)
        throw std::runtime_error("Only one server instance can exist.");
    return instance;
}

// -- init ----
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

void Server::initKqueue()
{
    _kq = kqueue();
    if (_kq == -1)
        throw std::runtime_error("Error: kqueue");
    const int signalList[] = { SIGINT, SIGQUIT, SIGTERM, SIGSTOP, SIGTSTP, SIGKILL};
    const size_t signalCount = sizeof(signalList) / sizeof(signalList[0]);
    size_t i;
    for (i = 0; i < signalCount; ++i)
    {
        EV_SET(_change+i, signalList[i], EVFILT_SIGNAL, EV_ADD | EV_ENABLE, 0, 0, NULL);
        signal(signalList[i], SIG_IGN);
    }
    EV_SET(_change+i, _socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kq, _change, signalCount+1, NULL, 0, NULL) == ERROR)
        throw std::runtime_error("Error: Kevent: Register server socket to KQueue");
}

void Server::initSocket()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    
    if (_socket < 0)
        throw std::runtime_error("socket() failed");

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("setsockopt() failed");
    
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0) // Shouldn't be necessary since we use kqueue, but we never know with macOS LOL
        throw std::runtime_error("fcntl() failed");
   
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);

    int bindAttempt = 0;
    const int maxBindAttempts = 3;
    const int retryIntervalSeconds = 2;    
    
    while (bind(_socket, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) == ERROR)
    {
        if (errno == EADDRINUSE && bindAttempt < maxBindAttempts)
        {
            std::cerr << "bind() failed, retrying in " << retryIntervalSeconds << "seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(retryIntervalSeconds));
            ++bindAttempt;
        }
        else
        {
            std::cerr << "bind() failed, giving up." << std::endl;
            exit(1);
        }
    }
    if (listen(_socket, _maxClients) < 0)
        throw std::runtime_error("listen() failed");
}

// -- end ----
Server::~Server()
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        closeClient(it->first);
    }
    std::map<std::string, Channel*>::iterator it2;
    for (it2 = _channels.begin(); it2 != _channels.end(); ++it2)
    {
        delete it2->second;
    }
    close(_kq);
    close(_socket);
}

void Server::closeClient(int socket)
{
    if (_clients[socket])
    {
        delete _clients[socket];
        _clients[socket] = NULL;
        _clients.erase(socket);
        try
        {
            editKevent(socket, EVFILT_READ, EV_DELETE, "deleting client read from kqueue");
            editKevent(socket, EVFILT_WRITE, EV_DELETE, "deleting client write from kqueue");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    CLOSE_CONNECTION_MSG(socket);
    close(socket);
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
    {
        channelsList.push_back(RPL_LIST(clientNick, it->second->getName(), it->second->getUsersCount(), it->second->getTopic()));
    }
    return channelsList;
}