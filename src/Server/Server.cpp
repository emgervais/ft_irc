#include "Server.hpp"
#include "NumericReplies.hpp"


// -- init --
Server::Server(int argc, char *argv[])
    : _maxClients(MAX_CLIENTS)
{
    try
    {
        setParams(argc, argv);
        initSocket();
        initKqueue();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl << USAGE << std::endl;
        exit(1);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

Server::Server(const Server& rhs)
{
    *this = rhs;
}

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs)
    {
        _socket = rhs._socket;
        _port = rhs._port;
        _maxClients = rhs._maxClients;
        _pass = rhs._pass;
        _addr = rhs._addr;
        _readFdSet = rhs._readFdSet;
        _writeFdSet = rhs._writeFdSet;
        _clients = rhs._clients;
        memcpy(_events, rhs._events, sizeof(_events));
        memcpy(_buffer, rhs._buffer, sizeof(_buffer));
    }
    return *this;
}

void    Server::setParams(int argc, char *argv[])
{
    if (argc != 3)
        throw std::invalid_argument("Error: wrong number of arguments");
    std::stringstream ss(argv[1]);
    ss >> _port;
    if (ss.fail() || !ss.eof() || _port < 0 || _port > 65535)
        throw std::invalid_argument("Error: wrong port number");
    ss.str(argv[2]);
    ss.clear();
    ss >> _pass;
    if (ss.fail() || !ss.eof())
        throw std::invalid_argument("Error: wrong password");
}

void Server::initSocket()
{

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1)
        throw std::runtime_error("Error: socket creation failed");

    int opt = 1;
    // Setsockopt Explanation at the end of this file (may be removed later)
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

Server::~Server()
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        CLOSE_CONNECTION(it->first);
        close(it->first);
        delete it->second;
    }
    _clients.clear();
    close(_socket);
}

void Server::closeClient(int socket)
{
    CLOSE_CONNECTION(socket);
    delete _clients[socket];
    _clients.erase(socket);
    close(socket);
}

void Server::closeServer()
{
    std::cout << "Closing server..." << std::endl;
    exit(0);
}

std::string Server::getPass() const
{
    return (_pass);
}


bool Server::isNicknameTaken(const std::string& nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNick() == nickname)
            return true;
    }
    return false;
}

void    Server::sendToClients(const std::string& msg, std::vector<int> sockets)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (std::find(sockets.begin(), sockets.end(), it->first) == sockets.end())
            writeToClient(it->first, msg);
    }
}

// Setsockopt Explanation
// The SO_REUSEADDR option allows a socket to bind to an address that is still in the TIME_WAIT state.
// When a socket is closed, it enters the TIME_WAIT state for a certain period to ensure that any delayed packets related to the closed connection are not misinterpreted by the operating system.

// Why It's Important:

// Without SO_REUSEADDR, if you try to bind a new socket to an address that is still in TIME_WAIT,
// the bind operation will fail. Enabling SO_REUSEADDR allows the reuse of the local address immediately after the socket is closed.
// This is particularly useful in server applications that might need to restart quickly or bind to the same address and port shortly after shutting down.

// Scenarios Where It's Useful:
// Server Restart:
// If your server needs to restart quickly after being shut down, you might encounter issues if the previous socket is still in TIME_WAIT.
// Enabling SO_REUSEADDR helps avoid delays in restarting the server.

// Frequent Binding to the Same Address:
// In some server applications, you might want to bind to the same address and port repeatedly.
// Enabling SO_REUSEADDR allows you to do this without waiting for the TIME_WAIT period to expire.