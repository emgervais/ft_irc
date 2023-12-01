#include "Server.hpp"

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

    std::cout << "Port: " << _port << std::endl;
    std::cout << "Password: " << _pass << std::endl;
    std::cout << "Server is listening on port " << _port << std::endl;
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
    
    sockaddr_in _addr;
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(_port);

    if (bind(_socket, (sockaddr *)&_addr, sizeof(_addr)) == -1)
        throw std::runtime_error("Error: socket bind failed");
    
    if (listen(_socket, 10) == -1)
        throw std::runtime_error("Error: socket listen failed");
}

void Server::initKqueue()
{
    _kqueue = kqueue();
    if (_kqueue == -1)
        throw std::runtime_error("Error: kqueue creation failed");
    EV_SET(&_events[0], _socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kqueue, &_events[0], 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("Error: kqueue event creation failed");

}

void    Server::initFdSet()
{
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


// -- run --

void Server::run()
{
    serverLoop();
}

void Server::serverLoop()
{
    while (true)
    {
        serverQueue();
        for (int i = 0; i < MAX_EVENTS; i++)
        {
            if (_events[i].flags == 0)
                continue;
            if ((int) _events[i].ident == _socket)
                registerNewClient();
            else if (_events[i].flags & EVFILT_READ)
                readFromClient(_events[i].ident);
            else if (_events[i].flags & EVFILT_WRITE)
                writeToClient(_events[i].ident);
        }
    }
}

void Server::serverQueue()
{
    int     ret;
    ret = kevent(_kqueue, NULL, 0, _events, MAX_EVENTS, NULL);
    if (ret == -1)
        throw std::runtime_error("Error: kqueue event creation failed");
}

void Server::writeToClient(int fd) {
    // Implement writing to the client with file descriptor fd
    // Example:
    // send(fd, dataBuffer, dataSize, 0);
}

void Server::readFromClient(int fd) {
    // Implement reading from the client with file descriptor fd
    // Example:
    // char buffer[1024];
    // int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
    // Handle received data...
}

void Server::registerNewClient() {
    // Accept a new client connection
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
    if (clientSocket == -1) {
        // Handle error while accepting client connection
        std::cerr << "Error accepting client connection" << std::endl;
        return;
    }

    // Add the new client to the kqueue for event monitoring
    EV_SET(_events, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1) {
        // Handle error adding client to kqueue
        std::cerr << "Error adding client to kqueue" << std::endl;
        close(clientSocket); // Close the client socket in case of error
        return;
    }


    Client newClient(clientSocket, "nickname", "username", *this);
    _clients.insert(std::pair<std::string, Client>(newClient.getNick(), newClient));
    std::cout << "New client: " << newClient.getNick() << std::endl;
}



// ----
Server::~Server()
{
}
