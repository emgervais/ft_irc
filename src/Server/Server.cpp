#include "Server.hpp"

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

void Server::initSocket()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    
    if (_socket < 0)
        throw std::runtime_error("socket() failed");

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("setsockopt() failed");
   
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);

    bindSocket();
    if (listen(_socket, _maxClients) < 0)
        throw std::runtime_error("listen() failed");
}

void Server::bindSocket()
{
    int i = 0;
    const int maxBindAttempts = 3;
    const int retryIntervalSeconds = 2;   

    while (bind(_socket, reinterpret_cast<sockaddr*>(&_addr), sizeof(_addr)) == ERROR)
    {
        if (errno == EADDRINUSE && i < maxBindAttempts)
        {
            std::cerr << "bind() failed, retrying in " << retryIntervalSeconds << "seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(retryIntervalSeconds));
            ++i;
        }
        else
        {
            std::cerr << "bind() failed, shutting down server." << std::endl;
            exit(1);
        }
    }
}

// -- end ----
Server::~Server()
{
    std::map<int, Client*>::iterator it;
    std::map<std::string, Channel*>::iterator it2;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        closeClient(it->first);
        // if (it->second)
        // {
        //     delete it->second;

        //     // try
        //     // {
        //     //     editKevent(it->first, EVFILT_READ, EV_DELETE, "deleting client read from kqueue");
        //     //     editKevent(it->first, EVFILT_WRITE, EV_DELETE, "deleting client write from kqueue");
        //     // }
        //     // catch(const std::exception& e)
        //     // {
        //     //     std::cerr << e.what() << '\n';
        //     // }
        // }
        // close(it->first);
    }
    
    for (it2 = _channels.begin(); it2 != _channels.end(); ++it2)
        if (it2->second)
            delete it2->second;

    try { editKevent(_socket, EVFILT_READ, EV_DELETE, "deleting server socket read from kqueue"); }
    catch(const std::exception& e)  { std::cerr << e.what() << '\n'; }

    close(_socket);
    close(_kq);
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
            CLOSE_CONNECTION_MSG(socket);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    close(socket);
}
