#include "Server.hpp"
#include "NumericReplies.hpp"
#include <signal.h>

std::vector<int> SOCKET_LIST;

void	sigint_handler(int sig)
{
    std::cout << std::endl << "SIGINT received" << std::endl;
    (void)sig;
    for (size_t i = 0; i < SOCKET_LIST.size(); i++)
    {
        if (SOCKET_LIST[i] != -1)
            close(SOCKET_LIST[i]);
    }
    exit(0);
}

void	init_signals()
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "Error: sigaction failed" << std::endl;
		exit(1);
	}
}

// -- init --
Server::Server(int argc, char *argv[])
    : _maxClients(MAX_CLIENTS)
{
    try
    {
        setParams(argc, argv);
        initSocket();
        initKqueue();
        init_signals();
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
    SOCKET_LIST.push_back(_socket);
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

void Server::registerNewClient()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
    SOCKET_LIST.push_back(clientSocket);
    if (clientSocket == -1)
    {
        std::cerr << "Error accepting client connection" << std::endl;
        return;
    }
    EV_SET(_events, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Error adding client to kqueue" << std::endl;
        close(clientSocket);
        return;
    }    
    Client newClient(clientSocket, *this);
    _clients.insert(std::make_pair(clientSocket, newClient));
}

void Server::writeToClient(int socket)
{
    // Implement writing to the client with file descriptor socket
    // Example:
    // send(socket, dataBuffer, dataSize, 0);
}

void Server::readFromClient(int socket)
{
    char _buffer[BUFF_SIZE];
    ssize_t bytesRead = recv(socket, _buffer, sizeof(_buffer) - 1, 0);
    if (bytesRead <= 0)
    {
        close(socket);
        return;
    }
    _buffer[bytesRead] = '\0';
    std::string data(_buffer);

    /* Plutôt que de retourner une string, parser le message et agir avec.
       Concernant registerNewClient, je crois qu'on devrait créer un client quelconque avec son socket.
       Ensuite, readFromClient devra remplir les informations du client.

       On devrait avoir une map (socket: Client)
        et peut-être une map (nickname: socket)
    */

    // Exemple de parsing IRC
    // ...
    // if (data.substr(0, 4) == "PING")
    // {
    //     std::string pingMessage = data.substr(5);
    //     send(fd, "PONG " + pingMessage + "\r\n",  // Sending PONG response
    //          strlen(("PONG " + pingMessage + "\r\n")), 0);
    // }

    // if PASS != _pass: disconnect
}



bool Server::isNicknameTaken(const std::string& nickname)
{
    std::map<int, Client>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        const Client& client = it->second;
        if (client.getNick() == nickname)
            return true;
    }
    return false;
}


void Server::sendErrorMessageToClient(int clientSocket, const std::string& errorMessage)
{
    const char* message = errorMessage.c_str();
    write(clientSocket, message, strlen(message));
}

// ----
Server::~Server()
{
    for (size_t i = 0; i < SOCKET_LIST.size(); i++)
    {
        if (SOCKET_LIST[i] != -1)
            close(SOCKET_LIST[i]);
    }
}
