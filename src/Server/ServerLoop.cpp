#include "Server.hpp"

// -- loop --
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
            else if (_events[i].filter == EVFILT_READ)
                readFromClient(_events[i].ident);
            else if (_events[i].filter == EVFILT_WRITE)
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
    EV_SET(_events, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Error: adding client to kqueue" << std::endl;
        close(clientSocket);
        return;
    }
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
    std::string msg = _buffer;
    _buffer[0] = '\0';
    Command cmd(*_clients[socket], *this, msg);
    cmd.exec();
    if (_clients[socket]->getReply().size() > 0)
    {
        EV_SET(_events, socket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
            std::cerr << "Error: adding client to kqueue" << std::endl;
    }
}

// -- send ----
void Server::writeToClient(int socket)
{
    std::string msg = _clients[socket]->getReply();

    ssize_t bytesSent = send(socket, msg.c_str(), msg.size(), 0);
    if (bytesSent == -1)
        std::cerr << "Error: sending to client" << std::endl;
    else if (bytesSent == 0)
        closeClient(socket);
    else
    {
        _clients[socket]->removeReply();
        FROM_SERVER(msg);
        if (_clients[socket]->getReply().size() == 0)
        {
            EV_SET(_events, socket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
            if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
                std::cerr << "Error: adding client to kqueue" << std::endl;
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
            EV_SET(_events, it->first, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
            if (kevent(_kqueue, _events, 1, NULL, 0, NULL) == -1)
                std::cerr << "Error: adding client to kqueue" << std::endl;
        }
    }
}
