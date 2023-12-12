#include "Server.hpp"
#include "IRC.hpp"
#include "util.hpp"
#include <cstring>

// -- loop ----
void Server::run()
{
    while (true)
    {
        int nev = serverQueue();
        for (int i = 0; i < nev; ++i)
        {
            if (_events[i].filter == EVFILT_SIGNAL)
                exitSignal(_events[i].ident);
            else if ((int) _events[i].ident == _socket)
                registerNewClient();
            else if (_events[i].filter == EVFILT_READ)
                readFromClient(_events[i].ident);
            else if (_events[i].filter == EVFILT_WRITE)
                writeToClient(_events[i].ident);
        }
        addWriteKevent();
    }
}

// -- receive ----
void Server::registerNewClient()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(_socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);

    if (clientSocket == -1)
    {
        std::cerr << "Error: accepting new client" << std::endl;
        return;
    }
    editKevent(clientSocket, EVFILT_READ, EV_ADD, "adding client read to kqueue");
    Client *newClient = new Client(clientSocket, *this);
    NEW_CONNECTION_MSG(clientSocket);
    _clients.insert(std::make_pair(clientSocket, newClient));
}

void Server::readFromClient(int socket)
{
    ssize_t bytesRead = recv(socket, _buffer, sizeof(_buffer) - 1, 0);
    if (bytesRead == -1)
        std::cerr << "Error: reading from client" << std::endl;
    else if (bytesRead == 0)
        closeClient(socket);
    else
    {
        _buffer[bytesRead] = '\0';
        FROM_CLIENT(std::string(_buffer));
        handleMsg(socket, bytesRead);
    }
}

void Server::handleMsg(int socket, ssize_t bytesRead)
{
    if (_buffer[bytesRead - 2] != '\r' || _buffer[bytesRead - 1] != '\n')
        return;
    std::vector<std::string> cmds = splitString(_buffer, "\r\n");
    _buffer[0] = '\0';

    for (size_t i = 0; i < cmds.size(); ++i)
    {
        Command cmd(*_clients[socket], *this, cmds[i]);
        cmd.exec();
    }    
}

// -- send ----
void Server::writeToClient(int socket)
{
    if (!_clients[socket])
        return;
    size_t repliesQty = _clients[socket]->getRepliesQty();
    for (size_t i = 0; i < repliesQty; ++i)
    {
        std::string msg = _clients[socket]->getReply();
        ssize_t bytesSent = send(socket, msg.c_str(), msg.size(), 0);
        if (bytesSent == ERROR)
            std::cerr << "Error: sending to client" << std::endl;
        else if (bytesSent == 0)
            closeClient(socket);
        else
        {
            FROM_SERVER(std::string(msg));
            _clients[socket]->removeReply();
        }
    }
}

void    Server::writeToClients(std::vector<int> sockets, const std::string& msg)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (std::find(sockets.begin(), sockets.end(), it->first) == sockets.end())
        {
            it->second->addReply(msg);
        }
    }
}

// -- Kevent ----
void Server::editKevent(int socket, int filter, int flags, std::string msg)
{
    EV_SET(_change, socket, filter, flags, 0, 0, NULL);
    if (kevent(_kq, _change, 1, NULL, 0, NULL) == ERROR)
    {
        bool deleteNonExistent = (errno == ENOENT);
        if (deleteNonExistent)
            return;
        else
            throw std::runtime_error("Error: " + msg);
    }
}

void Server::addWriteKevent()
{
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second && !it->second->getReply().empty())
            editKevent(it->first, EVFILT_WRITE, EV_ADD | EV_ONESHOT, "adding client write to kqueue");
    }
}

int Server::serverQueue()
{
    int evQty = kevent(_kq, NULL, 0, _events, MAX_EVENTS, NULL);
    if (evQty == ERROR)
        throw std::runtime_error("Error: kqueue event creation failed");
    return evQty;
}

// -- exit ----
void Server::exitSignal(int sig)
{
    std::cout << "Quitting due to signal reception: ";
    const char *signal_name = strsignal(sig);
    if (signal_name != nullptr) {
        std::cout << signal_name << " (" << sig << ")" << std::endl;
    } else {
        std::cout << "Unknown Signal (" << sig << ")" << std::endl;
    }
    exit(128 + sig);
}
