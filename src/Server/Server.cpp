#include "Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../Commands/Command.hpp"
#include <thread>
#include <unistd.h>
#include <fcntl.h>

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
    : _port(port), _pass(password)
{
    try
    {
        initSocket();
        initKqueue();
        loadSwearWords();
        initEquivalentChars();
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
    
    if (_socket == ERROR)
        throw std::runtime_error("socket() failed");

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw std::runtime_error("setsockopt() failed");
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_port);

    fcntl(_socket, F_SETFL, O_NONBLOCK);        
    bindSocket(addr);
    if (listen(_socket, MAX_CLIENTS) == ERROR)
        throw std::runtime_error("listen() failed");
}

void Server::bindSocket(sockaddr_in const& addr)
{
    int i = 0;
    const int MAX_BINDS = 3;
    const int RETRY_INTERVAL = 2;   

    while (bind(_socket, (struct sockaddr*)&addr, sizeof(addr)) == ERROR)
    {
        if (errno == EADDRINUSE && i < MAX_BINDS)
        {
            std::cerr << "bind() failed, retrying in " << RETRY_INTERVAL << "seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(RETRY_INTERVAL));
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
    std::map<int, Client*>::iterator clientIt;
    for (clientIt = _clients.begin(); clientIt != _clients.end(); ++clientIt)
        closeClient(clientIt->first, false);

    try
    {
        editKevent(_socket, EVFILT_READ, EV_DELETE, "deleting server socket read from kqueue");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        editKevent(fileno(stdin), EVFILT_READ, EV_DELETE, "deleting stdin read from kqueue");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    close(_socket);
    close(_kq);
}

void Server::closeClient(int socket, bool eraseFromMap)
{
    CLOSE_CONNECTION_MSG(socket);
    delete _clients[socket];
    if (eraseFromMap)
        _clients.erase(socket);
    else
        _clients[socket] = NULL;
    try
    {
        editKevent(socket, EVFILT_READ, EV_DELETE, "deleting client read from kqueue");
        editKevent(socket, EVFILT_WRITE, EV_DELETE, "deleting client write from kqueue");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    close(socket);
}
