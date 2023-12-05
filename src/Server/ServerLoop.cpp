#include "Server.hpp"

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
    NEW_CONNECTION(clientSocket);
    _clients.insert(std::make_pair(clientSocket, newClient));
}

void Server::writeToClient(int socket)
{
    ssize_t bytesSent = send(socket, _buffer, strlen(_buffer), 0);

    if (bytesSent == -1)
        std::cerr << "Error: sending message to client" << std::endl;
    else if (bytesSent == 0)
        closeClient(socket);
    else
        FROM_SERVER(std::string(_buffer));
    _buffer[0] = '\0';
}

void Server::writeToClient(int socket, const std::string& msg)
{
    strncpy(_buffer, msg.c_str(), msg.size());
    writeToClient(socket);
}

void Server::handleMsg(int socket, ssize_t bytesRead)
{
    // So that we can use nc without having to type \r\n
    // if (_buffer[bytesRead - 2] != '\r' || _buffer[bytesRead - 1] != '\n')
    //     throw std::invalid_argument("Invalid command: not terminated by \\r\\n");

    // Will be removed when we use a real client
    if (!(_buffer[bytesRead - 1] != '\n' || _buffer[0] == '\n'))
    {
        if (_buffer[bytesRead - 1] == '\r')
            _buffer[bytesRead - 1] = '\0';
        else
            _buffer[bytesRead] = '\0';
        try
        {
            Command cmd(*_clients[socket], *this, _buffer);
            cmd.exec();
            if (!cmd.getReply().empty())
                writeToClient(socket, cmd.getReply());
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << "Error: handling message from client" << std::endl;
            writeToClient(socket, e.what());
        } 
    }
    _buffer[0] = '\0';
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
