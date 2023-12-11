#include "Server.hpp"
#include "util.hpp"

// -- loop ----
void Server::run()
{
    while (true)
    {
        int nev = serverQueue();
        for (int i = 0; i < nev; ++i)
        {
            if ((int) _events[i].ident == _socket)
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
    if (_buffer[bytesRead - 1] != '\n' || _buffer[bytesRead - 2] != '\r')
        return;
    std::string raw(_buffer);
    raw = raw.substr(0, bytesRead - 2);
    Command cmd(*_clients[socket], *this, raw);
    cmd.exec();
}

// -- send ----
void Server::writeToClient(int socket)
{
    if (!_clients[socket])
        return;
    std::string msg = _clients[socket]->getReply();
    if(msg.empty())
        return ;
    ssize_t bytesSent = send(socket, msg.c_str(), msg.size(), 0);
    if (bytesSent == -1)
        std::cerr << "Error: sending to client" << std::endl;
    else if (bytesSent == 0)
        closeClient(socket);
    else
    {
        FROM_SERVER(std::string(msg));
        _clients[socket]->removeReply();
        if (!_clients[socket]->getReply().empty())
            writeToClient(socket);
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
    EV_SET(&_change, socket, filter, flags, 0, 0, NULL);
    if (kevent(_kq, &_change, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("Error: " + msg);
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
