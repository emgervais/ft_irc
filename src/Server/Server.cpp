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
        Command::initCmdHandler();
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

// void Server::closeServer()
// {
//     std::cout << "Closing server..." << std::endl;
//     exit(0);
// }

// -- misc ----
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

bool Server::isChannelNameTaken(const std::string& channelName)
{
    return (_channels.find(channelName) != _channels.end());
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
