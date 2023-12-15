#include "Server.hpp"
#include "../Channel/Channel.hpp"
#include "../Client/Client.hpp"
#include "../Commands/Command.hpp"
#include <thread>
#include <unistd.h>

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

// void Server::initBot()
// {
//     delete _bot;
//     int s = socket(AF_INET, SOCK_STREAM, 0);
//     if (s == ERROR)
//         throw std::runtime_error("socket() failed");
//     int optval = 1;
//     if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
//     {
//         close(s);
//         throw std::runtime_error("setsockopt() failed");
//     }
//     if (connect(s, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
//         close(s);
//         throw std::runtime_error("connect() failed");
//     }
//     _bot = new Client(s, *this);
//     _bot->checkPassword(_pass);
//     _bot->setNick("JT");
//     std::vector<std::string> user;
//     user.push_back("JohnTravolta");
//     user.push_back("*");
//     user.push_back("0");
//     user.push_back("John Travolta");
//     _bot->setUser(user);
//     _bot->disableWaitingForPong();
// }

// -- end ----
Server::~Server()
{
    std::map<std::string, Channel*>::iterator it2;

    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
        closeClient(it->first, true);
    for (it2 = _channels.begin(); it2 != _channels.end(); ++it2)
        delete it2->second;
    try { editKevent(_socket, EVFILT_READ, EV_DELETE, "deleting server socket read from kqueue"); }
    catch(const std::exception& e)  { std::cerr << e.what() << '\n'; }
    close(_socket);
    close(_kq);
}

void Server::closeClient(int socket, bool serverClosing)
{
    if (_clients[socket])
    {
        delete _clients[socket];
        if (!serverClosing)
        {
            _clients.erase(socket);
            _clients[socket] = NULL;
        }
    }
    try
    {
        editKevent(socket, EVFILT_READ, EV_DELETE, "deleting client read from kqueue");
        editKevent(socket, EVFILT_WRITE, EV_DELETE, "deleting client write from kqueue");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    close(socket);
    CLOSE_CONNECTION_MSG(socket);
}
