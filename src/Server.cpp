#include "Server.hpp"

// ----
Server::Server(int argc, char **argv)
    : _maxClients(MAX_CLIENTS)
{
    try
    {
        checkParam(argc, argv);
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
    // std::cout << "Port: " << _port << std::endl;
    // std::cout << "Password: " << _pass << std::endl;
    // std::cout << "Server is listening on port " << _port << std::endl;
}

void    Server::checkParam(int argc, char **argv)
{
    if (argc != 3)
        throw std::invalid_argument("Error: wrong number of arguments");
    std::stringstream ss(argv[1]);
    ss >> std::noskipws >> _port;
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
// ----

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

// ----
Server::~Server()
{
    closeServer();
}
