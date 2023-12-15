#include "Server.hpp"
#include "../util/util.hpp"
#include "../Commands/Command.hpp"
#include "../Client/Client.hpp"
#include <iostream>
#include <unistd.h>

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
            else if ((int) _events[i].ident == _socket && _clients.size() < MAX_CLIENTS)
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
    try
    {
        editKevent(clientSocket, EVFILT_READ, EV_ADD, "adding client read to kqueue");
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Client connection failed. Disconnecting." << std::endl;
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
    {
        std::cerr << "Error: reading from client" << std::endl;
        closeClient(socket);
    }
    else if (bytesRead == 0)
        closeClient(socket);
    else
    {
        _buffer[bytesRead] = '\0';
        std::string msg(_buffer);
        if (msg.find("PONG") == std::string::npos)
            FROM_CLIENT(std::string(_buffer));
        handleMsg(socket, bytesRead);
    }
}

void Server::handleMsg(int socket, ssize_t bytesRead)
{
    std::vector<std::string> cmds;
    if (_buffer[bytesRead - 1] == '\n' && _buffer[bytesRead - 2] == '\r')
        cmds = splitString(_buffer, "\r\n");
    else if (_buffer[bytesRead - 1] == '\n')
        cmds = splitString(_buffer, "\n");
    else
        return;
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
        {
            std::cerr << "Error: sending to client" << std::endl;
            closeClient(socket);
        }
        else if (bytesSent == 0)
            closeClient(socket);
        else
        {
            FROM_SERVER(std::string(msg));
            _clients[socket]->removeReply();
        }
    }
    if (_clients[socket]->isClosing())
            closeClient(socket);
}

// -- exit ----
void Server::exitSignal(int sig)
{
    const char *signal_name = strsignal(sig);

    std::cout << std::endl << "Quitting due to signal reception: ";
    if (signal_name != nullptr)
        std::cout << signal_name << " (" << sig << ")" << std::endl;
    else
        std::cout << "Unknown Signal (" << sig << ")" << std::endl;

    exit(128 + sig);
}
